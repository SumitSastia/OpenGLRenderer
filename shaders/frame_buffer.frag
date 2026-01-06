#version 330 core

in vec2 vTexCords;
out vec4 FragColor;

uniform sampler2D screen;

void main() {
	FragColor = texture(screen, vTexCords);
}