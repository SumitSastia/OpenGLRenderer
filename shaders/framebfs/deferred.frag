#version 330 core

in vec2 vTexCords;
out vec4 FragColor;

const int MAX_LIGHTS = 16;

struct light {
	vec3 position;
	vec3 color;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gTexture;
uniform light lights[MAX_LIGHTS];
uniform vec3 viewPos;

void main() {

	vec3 fragPos = texture(gPosition, vTexCords).rgb;
	vec3 normal = texture(gNormal, vTexCords).rgb;
	vec3 albedo = texture(gTexture, vTexCords).rgb;

	float specular = texture(gTexture, vTexCords).a;

	// Ambient Lighting
	vec3 ambient = albedo * 0.2;
	vec3 finalColor = vec3(0.0);

	vec3 viewDirection = normalize(viewPos - fragPos);

	for (int i = 0; i < 1; i++) {

		vec3 lightDirection = normalize(lights[i].position - fragPos);
		vec3 diffuse = max(dot(normal, lightDirection), 0.0) * albedo * lights[i].color;
		finalColor += diffuse;
	}

	FragColor = vec4(ambient + finalColor, 1.0);
}