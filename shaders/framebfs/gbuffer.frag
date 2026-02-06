#version 330 core
#define MAX_LIGHTS 5

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gTexture;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCords;

uniform sampler2D texture1;
uniform sampler2D texture2;

/***************************************************************************/


/***************************************************************************/

void main() {

	gPosition = vec4(vPos, 1.0);
	gNormal = vec4((vNormal), 1.0);
	gNormal = vec4(0,1,0,1);

	gTexture.rgb = texture(texture1, vTexCords).rgb;
	// gTexture.a = texture(texture2, vTexCords).r;
	gTexture.a = 1.0;
}