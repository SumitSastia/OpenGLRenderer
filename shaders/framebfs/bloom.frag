#version 450 core

in vec2 vTexCords;
out vec4 FragColor;

uniform sampler2D screen;
uniform sampler2D bloom;

void main() {
	
	const float bloomStrength = 0.1;

	FragColor = texture(screen, vTexCords);

	FragColor.rgb += bloomStrength * texture(bloom, vTexCords).rgb;
}