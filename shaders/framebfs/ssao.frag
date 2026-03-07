#version 330 core

in vec2 vTexCords;
// out vec4 FragColor;
out float FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gTexture;
// uniform sampler2D noiseTexture;

uniform vec3 samples[64];
uniform mat4 projection;

const vec2 noiseScale = vec2(1120.0/4.0, 700.0/4.0);

void main() {

	vec3 fragPos = texture(gPosition, vTexCords).rgb;
	vec3 normal  = texture(gNormal, vTexCords).rgb;
	vec3 albedo  = texture(gTexture, vTexCords).rgb;

	// vec3 randomVec = texture(noiseTexture, vTexCords * noiseScale).xyz;
	vec3 randomVec = texture(gTexture, vTexCords * noiseScale).xyz;

	vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN	   = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;
	float bias      = 0.025;
	float radius    = 0.5;

	for (int i = 0; i < 64; i++) {

		vec3 _sample = TBN * samples[i];
		_sample = fragPos + _sample * radius;

		vec4 offset = vec4(_sample, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sampleDepth = texture(gPosition, offset.xy).z;
		occlusion += (sampleDepth >= _sample.z + bias)? 1.0 : 0.0;
	}

	// FragColor = vec4(albedo, 1.0);
	FragColor = occlusion;
}