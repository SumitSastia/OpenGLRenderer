#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCords;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCords;

uniform mat4 model;
uniform mat4 finalMatrix;
uniform mat3 normalModel;

void main(){

    gl_Position = finalMatrix * vec4(aPos, 1.0);
    vTexCords = aTexCords;

    vPos = vec3(model * vec4(aPos, 1.0));
    vNormal = normalModel * aNormal;
}
