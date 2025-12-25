#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 vColor;

uniform mat4 finalMatrix;

void main(){
    gl_Position = finalMatrix * vec4(aPos, 1.0);
    vColor = aColor;
}
