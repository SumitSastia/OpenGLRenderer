#version 450 core

struct material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct lightFade{
    float constant;
    float linear;
    float quadratic;
};

struct spotLight{
    vec3 position;
    vec3 direction;
    float cutOffangle;
    float outerCutOff;
};

in vec3 vPos;
in vec2 vTextureCords;
in vec3 vNormal;

out vec4 FragColor;

uniform float alphaVal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform material m1;
uniform lightFade l1;
uniform spotLight s1;

void main(){

    // Ignoring Cube Light-Source

    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(s1.position - vPos);

    vec3 t1 = vec3(texture(texture1, vTextureCords));
    vec3 t2 = vec3(texture(texture2, vTextureCords));

    // Ambient
    vec3 ambientLight = (m1.ambient*t1) * lightColor;

    float theta = dot(lightDirection, normalize(-s1.direction));

    // Diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * (m1.diffuse*(t1-t2) + t2) * lightColor;

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), m1.shininess);
    vec3 specularLight = spec * t2 * lightColor;

    // Attenuation
    float fragDistance = length(s1.position - vPos);
    float attenuation = 1.0 / (l1.constant + l1.linear*fragDistance + l1.quadratic*fragDistance*fragDistance);

    // Spotlight
    float epsilon = s1.cutOffangle - s1.outerCutOff;
    float intensity = clamp((theta - s1.outerCutOff) / epsilon, 0.0, 1.0);

    FragColor = vec4(ambientLight + (attenuation*intensity)*(diffuseLight + specularLight), 1.0);
}