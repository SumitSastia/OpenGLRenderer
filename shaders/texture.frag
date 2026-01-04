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

    float cutOffangle;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;
};

in vec3 vPos;
in vec3 vNormal;
in vec2 vTextureCords;

out vec4 FragColor;

uniform float alphaVal;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform material m1;

uniform spotLight s1;
uniform directionalLight d1;
uniform pointLight p1;

uniform bool useFlashLight;

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

    // Ignoring Cube Light-Source

    vec3 normal = normalize(vNormal);

    vec3 t1 = vec3(texture(texture1, vTextureCords));
    vec3 t2 = vec3(texture(texture1, vTextureCords));

    // Ambient
    vec3 ambientLight = (m1.ambient*t1) * vec3(1.0,1.0,1.0);

    vec3 finalColor = vec3(0.0);

    finalColor += init_pointLight(p1, normal, vPos, viewPos, t1, t2);
    // finalColor += init_directionalLight(d1, normal, vPos, viewPos, t1, t2);

    if(useFlashLight){
        finalColor += init_spotLight(s1, normal, vPos, viewPos, t1, t2);
    }

    FragColor = vec4(ambientLight + finalColor, 1.0);
}