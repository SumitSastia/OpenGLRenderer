#version 450 core

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

uniform samplerCube depthCubeMap;
uniform vec3 lightPos;
uniform float far_plane;

out vec4 FragColor;

uniform float alphaVal;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform material m1;

uniform spotLight s1;
uniform directionalLight d1;
uniform pointLight p1;

uniform float skyboxIntensity;
uniform samplerCube skybox;

float init_shadow(vec3 vPos) {

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

void main(){

    vec3 normal = normalize(vNormal);
    
    vec3 t1 = texture(texture1, vTextureCords).rgb;
    vec3 t2 = texture(texture2, vTextureCords).rgb;

    float alpha = texture(texture1, vTextureCords).a;

    // Ambient
    vec3 ambientLight = m1.ambient * t1;
    vec3 finalColor = vec3(0.0);

    finalColor += init_pointLight(p1, normal, vPos, viewPos, t1, t2);
    finalColor *= 1.0 - init_shadow(vPos);

    // finalColor += init_directionalLight(d1, normal, vPos, viewPos, t1, t2);

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