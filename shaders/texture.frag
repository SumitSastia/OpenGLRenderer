#version 450 core

in vec2 vTextureCords;
in vec3 vNormal;
in vec3 fragPos;

out vec4 FragColor;

uniform float alphaVal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D texture1;

void main(){

    float ambientStrength = 0.2;
    float specularStrength = 0.5;

    // Ambient
    vec3 ambientLight = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * lightColor;

    // Specular
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specularLight = specularStrength * spec * lightColor;

    // Final Color
    vec3 temp = vec3(1.0, 0.5, 0.31);
    vec3 resultColor = (ambientLight + diffuseLight + specularLight) * vec3(1.0,1.0,1.0);
    // FragColor = vec4(resultColor, 1.0);

    FragColor = texture(texture1, vTextureCords) * vec4(resultColor, 1.0);
    // FragColor = vec4(temp,1.0);
}