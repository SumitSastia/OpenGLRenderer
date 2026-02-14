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

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main(){

    FragColor = texture(texture1, vTextureCords);
}