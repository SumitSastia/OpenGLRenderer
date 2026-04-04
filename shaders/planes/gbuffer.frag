#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gTexture;

in vec3 vPos;
in vec2 vTexCords;

uniform vec3 normal;
uniform sampler2D texture1;

/***************************************************************************/

void main() {

	gPosition = vec4(vPos, 1.0);
	gNormal = vec4(normal, 1.0);

	gTexture.rgb = texture(texture1, vTexCords).rgb;
	gTexture.a = texture(texture1, vTexCords).r;

	gTexture.rgb = vec3(0.9);
}