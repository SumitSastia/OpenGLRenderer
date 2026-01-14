#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTextureCords;
layout (location = 3) in mat4 instanceModel;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTextureCords;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 offsetModels[10];
uniform mat3 offsetNormals[10];

void main(){

    mat4 myModel = mat4(1.0);

    // gl_Position = projection * view * offsetModels[gl_InstanceID] * vec4(aPos , 1.0);
    // gl_Position = projection * view * myModel * vec4(aPos , 1.0);
    gl_Position = projection * view * instanceModel * vec4(aPos , 1.0);

    vPos = vec3(instanceModel * vec4(aPos , 1.0));
    // vNormal = offsetNormals[gl_InstanceID] * aNormal;
    vNormal = aNormal;
    vTextureCords = aTextureCords;
}
