#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCords;

out vec2 vTexCords;
uniform mat4 model;

void main() {
	gl_Position = model * vec4(aPos, 0.0, 1.0);
	vTexCords = aTexCords;
}