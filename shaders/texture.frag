#version 450 core

struct material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
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

void main(){

    vec3 t1 = vec3(texture(texture1, vTextureCords));
    vec3 t2 = vec3(texture(texture2, vTextureCords));

    // Ambient
    vec3 ambientLight = m1.ambient * lightColor * t1;

    // Diffuse
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - vPos);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * (m1.diffuse*t1 + t2) * lightColor;

    // Specular
    vec3 viewDirection = normalize(viewPos - vPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specularLight = spec * vec3(texture(texture2, vTextureCords)) * lightColor;

    // Final Color
    // vec3 resultColor = (ambientLight + diffuseLight + specularLight) * vec3(1.0,1.0,1.0);

    FragColor = vec4((ambientLight + diffuseLight + specularLight), 1.0);
    // FragColor = vec4(resultColor,1.0);
}