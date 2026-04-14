#version 330 core

const int MAX_LIGHTS = 8;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gTexture;

in vec3 vPos;
in vec3 vNormal;
in vec2 vTexCords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int light_count;
uniform float metallic;
uniform float roughness;

uniform vec3 camPos;
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightColor[MAX_LIGHTS];

const float pi = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnalSchlick(float cosTheta, vec3 F0);

/***************************************************************************/

void main() {

	gPosition = vec4(vPos, 1.0);
	gNormal = vec4(vNormal, 1.0);

	gTexture.rgb = texture(texture1, vTexCords).rgb;
	gTexture.a = texture(texture2, vTexCords).r;

	// For Ambient Occlusion
	// gTexture = vec4(vec3(0.9), 1.0);

	vec3 N = normalize(vNormal.xyz);
	vec3 V = normalize(camPos - gPosition.xyz);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, gTexture.rgb, metallic);

	// reflectance
	vec3 Lo = vec3(0.0);

	for (int i = 0; i < light_count; i++) { 
		
		// calculate per-light radiance
		vec3 L = normalize(lightPosition[i] - vPos);
		vec3 H = normalize(V + L);

		float distance0   = length(lightPosition[i] - vPos);
		float attenuation = 1.0 / (distance0 * distance0);
		vec3 radiance     = lightColor[i] * attenuation;

		// cook-torrance BRDF
		float NDF  = distributionGGX(N, H, roughness);
		float G    = geometrySmith(N, V, L, roughness);
		vec3 F     = fresnalSchlick(max(dot(H, V), 0.0), F0);

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 numerator    = NDF * G * F;
		float denominator = 4.0 * max(dot(N,V), 0.0) * max(dot(N,L), 0.0) + 0.0001;
		vec3 specular     = numerator / denominator;
		
		// add specular to outgoing radiance
		float NdotL = max(dot(N,L), 0.0);
		Lo += (kD * gTexture.rgb / pi + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * gTexture.rgb;
	vec3 color   = ambient + Lo;
	
	color = color / (color + vec3(1.0));
	// Gamma-Correction
	// color = pow(color, vec3(1.0/2.2));

	gTexture.rgb = color;
}

/***************************************************************************/

float distributionGGX(vec3 N, vec3 H, float roughness) {

	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H), 0.0);
	float NdotH2 =  NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = pi * denom * denom;

	return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
	
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {

	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);

	float ggx2 = geometrySchlickGGX(NdotV, roughness);
	float ggx1 = geometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnalSchlick(float cosTheta, vec3 F0) {

	return F0 +	(1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}