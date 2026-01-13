#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCords;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTextureCords;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 offsetModels[10];
uniform mat3 offsetNormals[10];

void main(){

    vec4 newPos = projection * view * offsetModels[gl_InstanceID] * vec4(aPos , 1.0);
    gl_Position = newPos;

    vPos = vec3(offsetModels[gl_InstanceID] * vec4(aPos , 1.0));
    vNormal = offsetNormals[gl_InstanceID] * aNormal;
    vTextureCords = aTextureCords;
}
