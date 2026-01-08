#version 330 core

in vec2 vTexCords;
out vec4 FragColor;

uniform sampler2D screen;

void main() {
	FragColor = vec4(vec3(1.0 - texture(screen, vTexCords)), 1.0);
}