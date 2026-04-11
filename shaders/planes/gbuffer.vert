#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCords;

out vec3 vPos;
out vec2 vTexCords;
out vec3 vNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 normal;

void main() {

	vec4 viewPos = view * model * vec4(aPos, 0.0, 1.0);
	// vPos = viewPos.xyz;
	vPos = (model * vec4(aPos, 0.0, 1.0)).xyz;

	// mat3 normalMatrix = transpose(inverse(mat3(model)));

	// mat3 normalMatrix = transpose(inverse(mat3(view * model)));
	// vNormal = normalMatrix * normal;
	vNormal = normalize(normal);

	vTexCords = aTexCords;
	gl_Position = projection * viewPos;
}