#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 gNormal;

uniform mat4 finalMatrix;
uniform mat3 normalModel;

void main(){

    gl_Position = finalMatrix * vec4(aPos, 1.0);
    gNormal = normalize(normalModel * aNormal);    
}
