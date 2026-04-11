#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCords;

out vec3 vPos;
out vec3 vNormal;
out vec2 vTexCords;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 model;
uniform mat3 normalModel;

void main() {

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vPos = vec3(model * vec4(aPos, 1.0));
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vNormal = normalMatrix * aNormal;

	// vNormal = normalModel * aNormal;
	vTexCords = aTexCords;
}