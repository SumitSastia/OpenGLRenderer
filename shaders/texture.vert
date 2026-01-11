#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCords;

out vec3 gPos;
out vec3 gNormal;
out vec2 gTextureCords;

uniform mat4 model;
uniform mat4 finalMatrix;
uniform mat3 normalModel;

void main(){

    gl_Position = finalMatrix * vec4(aPos, 1.0);
    gTextureCords = aTextureCords;

    gPos = vec3(model * vec4(aPos, 1.0));
    gNormal = normalModel * aNormal;
    
}
