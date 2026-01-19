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

in vec4 lightSpacePos;
uniform sampler2D depthMap;

in vec3 vPos;
in vec2 vTexCords;
out vec4 FragColor;

uniform vec3 normal;
uniform vec3 viewPos;

uniform material m1;

uniform spotLight s1;
uniform directionalLight d1;
uniform pointLight p1;

uniform sampler2D texture1;
uniform float skyboxIntensity;

float init_shadow(vec4 lightSpacePos, sampler2D depthMap, vec3 normal, vec3 lightDirection) {

    if (lightSpacePos.w <= 0.0) {
        return 0.0;
    }

    vec3 shadowCords = lightSpacePos.xyz / lightSpacePos.w;
    shadowCords = shadowCords * 0.5 + 0.5;

    if (shadowCords.x < 0.0 || shadowCords.x > 1.0 ||
        shadowCords.y < 0.0 || shadowCords.y > 1.0 ||
        shadowCords.z < 0.0 || shadowCords.z > 1.0) {
        return 0.0;
    }

    float nearest_depth = texture(depthMap, shadowCords.xy).r;
    float shadow = 0.0;

    vec3 final_normal = normalize(normal);

    float bias = max(
        0.005 * (1.0 - dot(final_normal, lightDirection)),
        0.0005
    );

    // float bias = 0.0025;

    if (shadowCords.z - bias > nearest_depth) {
        shadow = 1.0;
    }

    return shadow;
}

vec4 init_pointLight(pointLight pl, vec3 normal, vec3 vPos, vec3 viewPos, vec4 t1){

    vec3 lightDirection = normalize(pl.position - vPos);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseLight = diff * vec4(m1.diffuse, 1.0) * t1 * vec4(pl.color, 1.0);

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    float spec = 0.0;

    if (m1.shininess > 2.0) {

        // Phong
        vec3 reflectDirection = reflect(-lightDirection, normal);
        spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    }
    else {

        // Blinn-Phong
        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        spec = pow(max(dot(viewDirection, halfwayDirection), 0.0), m1.shininess);
    }

    vec4 specularLight = spec * t1 * vec4(pl.color, 1.0);

    // Attenuation
    float fragDistance = length(pl.position - vPos);
    float attenuation = 1.0 / (pl.constant + pl.linear*fragDistance + pl.quadratic*fragDistance*fragDistance);

    return (attenuation * vec4(diffuseLight + specularLight));
}

vec4 init_directionalLight(directionalLight dl, vec3 normal, vec3 vPos, vec3 viewPos, vec4 t1){

    vec3 lightDirection = normalize(dl.direction);

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseLight = diff * (m1.diffuse, 1.0) * t1 * (dl.color, 1.0);

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec4 specularLight = spec * t1 * (dl.color, 1.0);

    return (diffuseLight + specularLight);
}

vec4 init_spotLight(spotLight sl, vec3 normal, vec3 vPos, vec3 viewPos, vec4 t1){

    vec3 lightDirection = normalize(sl.position - vPos);

    float theta = dot(lightDirection, normalize(-sl.direction));

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec4 diffuseLight = diff * vec4(m1.diffuse, 1.0) * t1 * vec4(sl.color, 1.0);

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec4 specularLight = spec * t1 * (sl.color, 1.0);

    // Attenuation
    float fragDistance = length(sl.position - vPos);
    float attenuation = 1.0 / (sl.constant + sl.linear*fragDistance + sl.quadratic*fragDistance*fragDistance);

    // Smooth-Edge
    float epsilon = sl.cutOffangle - sl.outerCutOff;
    float intensity = clamp((theta - sl.outerCutOff) / epsilon, 0.0, 1.0);

    return (attenuation * intensity * (diffuseLight + specularLight));
}

void main() {

    vec3 final_normal = normalize(normal);
    vec4 t1 = vec4(texture(texture1, vTexCords));

    // Ambient
    vec4 ambientLight = ((vec4(m1.ambient, 1.0) * t1) + skyboxIntensity * t1) / 2.00;
    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    // PointLight
    finalColor += init_pointLight(p1, normal, vPos, viewPos, t1);

    // SpotLight
    if(s1.isVisible){
        finalColor += init_spotLight(s1, normal, vPos, viewPos, t1);
    }

    vec3 lightDirection = normalize(p1.position - vPos);
    
    FragColor = ambientLight + (1.0 - init_shadow(lightSpacePos, depthMap, normal, lightDirection)) * finalColor;
    // FragColor = ambientLight + (1.0 - init_shadow(p1)) * finalColor;
    // FragColor = vec4(vec3(1.0 - init_shadow(p1)), 1.0);
}