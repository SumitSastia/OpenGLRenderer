#version 450 core
#define MAX_LIGHTS 3

struct material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct directionalLight{
    vec3 direction;
    vec3 color;
};

struct pointLight{
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

struct spotLight{
    vec3 position;
    vec3 direction;
    vec3 color;

    bool isVisible;

    float cutOffangle;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vTextureCords;



//----------------------------------------------------------------//

uniform samplerCube depthCubeMap[MAX_LIGHTS];
uniform vec3 lightPos[MAX_LIGHTS];
uniform vec3 viewPos;
uniform float far_plane;

uniform int lights_count;
uniform pointLight plights[MAX_LIGHTS];

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform material m1;

uniform spotLight s1;
uniform directionalLight d1;

uniform float skyboxIntensity;
uniform samplerCube skybox;

float init_shadow(vec3 vPos, samplerCube depthCubeMap, vec3 lightPos) {

    vec3 fragToLight = vPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;

    for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {

                float closestDepth = texture(depthCubeMap, fragToLight + vec3(x,y,z)).r;
                closestDepth *= far_plane;

                if (currentDepth - bias > closestDepth) {
                    shadow += 1.0;
                }
            }
        }
    }

    shadow /= (samples * samples * samples);

    return shadow;
}

vec3 init_pointLight(pointLight pl, vec3 normal, vec3 vPos, vec3 viewPos, vec3 t1, vec3 t2){

    vec3 lightDirection = normalize(pl.position - vPos);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * (m1.diffuse*(t1-t2) + t2) * pl.color;

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec3 specularLight = spec * t2 * pl.color;

    // Attenuation
    float fragDistance = length(pl.position - vPos);
    float attenuation = 1.0 / (pl.constant + pl.linear*fragDistance + pl.quadratic*fragDistance*fragDistance);

    return (attenuation * vec3(diffuseLight + specularLight));
}

vec3 init_directionalLight(directionalLight dl, vec3 normal, vec3 vPos, vec3 viewPos, vec3 t1, vec3 t2){

    vec3 lightDirection = normalize(dl.direction);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * (m1.diffuse*(t1-t2) + t2) * dl.color;

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec3 specularLight = spec * t2 * dl.color;

    return vec3(diffuseLight + specularLight);
}

vec3 init_spotLight(spotLight sl, vec3 normal, vec3 vPos, vec3 viewPos, vec3 t1, vec3 t2){

    vec3 lightDirection = normalize(sl.position - vPos);

    float theta = dot(lightDirection, normalize(-sl.direction));

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * (m1.diffuse*(t1-t2) + t2) * sl.color;

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec3 specularLight = spec * t2 * sl.color;

    // Attenuation
    float fragDistance = length(sl.position - vPos);
    float attenuation = 1.0 / (sl.constant + sl.linear*fragDistance + sl.quadratic*fragDistance*fragDistance);

    // Smooth-Edge
    float epsilon = sl.cutOffangle - sl.outerCutOff;
    float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);

    return (attenuation * intensity * vec3(diffuseLight + specularLight));
}

uniform float metallic;
uniform float roughness;

const float pi = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnalSchlick(float cosTheta, vec3 F0);

/***************************************************************************/

void main(){

    vec3 normal = normalize(vNormal);
    
    vec3 t1 = texture(texture1, vTextureCords).rgb;
    vec3 t2 = texture(texture2, vTextureCords).rgb;

    float alpha = texture(texture1, vTextureCords).a;

    // Ambient
    vec3 ambientLight = m1.ambient * t1;

    vec3 finalColor = vec3(0.0);
    vec3 lightColors[MAX_LIGHTS];

    // PointLight
    for (int i = 0; i < lights_count; i++) {
        
        lightColors[i] = init_pointLight(plights[i], normal, vPos, viewPos, t1, t2);
        lightColors[i] *= (1.0 - init_shadow(vPos, depthCubeMap[i], lightPos[i]));
    }

    for (int i = 0; i < lights_count; i++) {
        finalColor += lightColors[i];
    }

    if(s1.isVisible){
        finalColor += init_spotLight(s1, normal, vPos, viewPos, t1, t2);
    }

    vec3 incident_ray = normalize(vPos - viewPos);

    // Reflection of Skybox
    vec3 reflected_ray = reflect(incident_ray, normal);
    vec4 reflected_color = vec4(texture(skybox, reflected_ray).rgb, 1.0);

    vec4 skybox_tex = texture(skybox, reflected_ray);

    ambientLight = vec3(((m1.ambient * t1) + skyboxIntensity * t1) / 2.00);
    FragColor = vec4(ambientLight + finalColor, alpha);

    // Bloom
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	
	if (brightness > 0.75) {
		BrightColor = FragColor;
	}
	else {
		BrightColor = vec4(0.0,0.0,0.0,1.0);
	}

    //-----------------------------------------------------------------------//

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - vPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, t1, metallic);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < lights_count; i++) { 
		
		// calculate per-light radiance
		vec3 L = normalize(lightPos[i] - vPos);
		vec3 H = normalize(V + L);

		float distance0   = length(lightPos[i] - vPos);
		float attenuation = 1.0 / (distance0 * distance0);
		vec3 radiance     = vec3(1.0) * attenuation; // vec3(1.0) -> WhiteLight (Color)

		// cook-torrance BRDF
		float NDF  = distributionGGX(N, H, roughness);
		float G    = geometrySmith(N, V, L, roughness);
		vec3 F     = fresnalSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N,V), 0.0) * max(dot(N,L), 0.0) + 0.0001;
		vec3 specular     = numerator / denominator;
		
		// add specular to outgoing radiance
		float NdotL = max(dot(N,L), 0.0);
		Lo += (kD * t1.rgb / pi + specular) * radiance * NdotL;
	}

    FragColor = vec4(ambientLight + Lo, 1.0);

    //-----------------------------------------------------------------------//

    // FragColor = vec4(vec3(1.0 - shadow), 1.0);
    
    // float gamma = 2.2;
    // FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));

    //if (gl_FrontFacing){
    //    FragColor = vec4(0.25 * vec3(texture(texture1, vTextureCords)), 1.0);
    //}

    // Refraction

    //float ratio = 1.00 / 1.52;
    //vec3 refracted_ray = refract(incident_ray, normal, ratio);
    //FragColor = vec4(texture(skybox, refracted_ray).rgb, 1.0);

}

/***************************************************************************/

float distributionGGX(vec3 N, vec3 H, float roughness) {

	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H), 0.0);
	float NdotH2 =  NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = pi * denom * denom;

	return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
	
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {

	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);

	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnalSchlick(float cosTheta, vec3 F0) {

	return F0 +	(1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}