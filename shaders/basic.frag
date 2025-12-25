#version 450 core

in vec3 vColor;
in vec3 vNormal;
in vec3 fragPos;

out vec4 FragColor;

uniform float alphaVal;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main(){

    float ambientStength = 0.2;

    // Ambient Lighting
    vec3 ambientLight = ambientStength * lightColor;

    // Diffuse Lighting
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(lightPos - fragPos);

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuseLight = diff * lightColor;

    // Final Color
    vec3 resultColor = (ambientLight + diffuseLight) * vColor;
    FragColor = vec4(resultColor, 1.0);
}