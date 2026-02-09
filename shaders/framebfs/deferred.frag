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

uniform int lights_count;
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

	for (int i = 0; i < lights_count; i++) {

		float fragDistance = length(lights[i].position - fragPos);
		float attenuation = 1.0 / (1.0 + 0.001*fragDistance + 0.016*fragDistance*fragDistance);

		if (attenuation > 0.0) {

			vec3 lightDirection = normalize(lights[i].position - fragPos);

			// Diffuse
			vec3 diffuse = max(dot(normal, lightDirection), 0.0) * albedo * lights[i].color;
			finalColor += diffuse;

			// Specular
			vec3 viewDirection = normalize(viewPos - fragPos);
			vec3 halfwayDirection = normalize(lightDirection + viewDirection);

			float spec = pow(max(dot(normal, halfwayDirection), 0.0), 32.0);
			finalColor += spec * specular * lights[i].color;

			// Attenuation
			finalColor *= attenuation;
		}
	}

	FragColor = vec4(ambient + finalColor, 1.0);
}