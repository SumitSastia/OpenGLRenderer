#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCords;

out vec3 vColor;
out vec2 vTexCords;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    // gl_Position = transform * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCords = aTexCords;
}
