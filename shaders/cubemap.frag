#version 330 core

in vec3 vTextureDir;
uniform samplerCube cubeMap;

out vec4 FragColor;

void main() {
	FragColor = texture(cubeMap, vTextureDir);
}