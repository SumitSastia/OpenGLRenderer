#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCords;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTextureCords;

out vec4 lightSpacePos;

uniform mat4 model;
uniform mat4 finalMatrix;
uniform mat4 lightSpace;
uniform mat3 normalModel;

void main(){

    gl_Position = finalMatrix * vec4(aPos, 1.0);
    lightSpacePos = lightSpace * model * vec4(aPos, 1.0);

    vPos = vec3(model * vec4(aPos, 1.0));
    vNormal = normalModel * aNormal;
    vTextureCords = aTextureCords;
    
}
