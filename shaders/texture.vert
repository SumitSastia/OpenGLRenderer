#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCords;
layout (location = 2) in vec3 aNormal;

out vec3 vPos;
out vec2 vTextureCords;
out vec3 vNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){

    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vTextureCords = aTextureCords;

    vPos = vec3(model * vec4(aPos, 1.0));
    vNormal = aNormal;
}
