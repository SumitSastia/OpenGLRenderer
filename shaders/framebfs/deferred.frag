#version 330 core

in vec2 vTexCords;
out vec4 FragColor;

const int MAX_LIGHTS = 16;

struct light {
	vec3 position;
	vec3 color;
	// samplerCube depthMap;
};

// Binded in g_buffer::render()
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gTexture;

// Set in scene1::render_final()
uniform int lights_count;
uniform float far_plane;
uniform light lights[MAX_LIGHTS];
uniform samplerCube depthCubeMap[MAX_LIGHTS];
uniform vec3 viewPos;

uniform float skyboxIntensity;

float init_shadow(vec3 vPos, samplerCube depthCubeMap, vec3 lightPos) {

    vec3 fragToLight = vPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;
    float samples = 4.0;
    float offset = 0.1;

    for (float x = -offset; x < offset; x += offset / (samples * 0.5)) {
        for (float y = -offset; y < offset; y += offset / (samples * 0.5)) {
            for (float z = -offset; z < offset; z += offset / (samples * 0.5)) {
	
                float closestDepth = texture(depthCubeMap, fragToLight + vec3(x,y,z)).r;
                closestDepth *= far_plane;
	
                if (currentDepth - bias > closestDepth) {
                    shadow += 1.0;
                }
            }
        }
    }
	
    shadow /= (samples * samples * samples);

    return shadow;
}

vec3 init_pointLight(light currentLight, vec3 normal, vec3 fragPos, vec3 albedo, float specular) {

	vec3 lightColor = vec3(0.0);

	float fragDistance = length(currentLight.position - fragPos);
	float attenuation = 1.0 / (1.0 + 0.001*fragDistance + 0.016*fragDistance*fragDistance);

	if (attenuation > 0.0) {

		vec3 lightDirection = normalize(currentLight.position - fragPos);

		// Diffuse
		vec3 diffuse = max(dot(normal, lightDirection), 0.0) * albedo * currentLight.color;
		lightColor += diffuse;

		// Specular
		vec3 viewDirection = normalize(viewPos - fragPos);
		vec3 halfwayDirection = normalize(lightDirection + viewDirection);

		float spec = pow(max(dot(normal, halfwayDirection), 0.0), 32.0);
		lightColor += spec * specular * currentLight.color;

		// Attenuation
		lightColor *= attenuation;
	}

	return lightColor;
}

void main() {

	vec3 fragPos = texture(gPosition, vTexCords).rgb;
	vec3 normal = texture(gNormal, vTexCords).rgb;
	vec3 albedo = texture(gTexture, vTexCords).rgb;

	float specular = texture(gTexture, vTexCords).a;

	// Ambient Lighting
	vec3 ambient = albedo * skyboxIntensity;

	vec3 finalColor = vec3(0.0);
	vec3 lightColors[MAX_LIGHTS];

	for (int i = 0; i < lights_count; i++) {

		lightColors[i] = init_pointLight(lights[i], normal, fragPos, albedo, specular);
        lightColors[i] *= (1.0 - init_shadow(fragPos, depthCubeMap[i], lights[i].position));
	}

	for (int i = 0; i < lights_count; i++) {
        finalColor += lightColors[i];
    }

	FragColor = vec4(ambient + finalColor, 1.0);
}