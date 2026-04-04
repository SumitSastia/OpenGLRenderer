#version 330 core

in vec2 vTexCords;
out float FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D noiseTexture;

uniform vec3 samples[64];
uniform mat4 projection;

int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

const vec2 noiseScale = vec2(1120.0/4.0, 700.0/4.0);

void main() {
	
	vec3 fragPos = texture(gPosition, vTexCords).rgb;
	vec3 normal = texture(gNormal, vTexCords).rgb;
	vec3 randomVec = normalize(texture(noiseTexture, vTexCords * noiseScale).rgb);	

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float occlusion = 0.0;

	for (int i = 0; i < kernelSize; i++) {
		
		vec3 samplePos = TBN * samples[i];
		samplePos = fragPos + samplePos * radius;

		vec4 offset =  vec4(samplePos, 1.0);
		offset = projection * offset;
		offset.xyz /= offset.w;
		offset.xyz = offset.xyz * 0.5 + 0.5;

		float sampleDepth = texture(gPosition, offset.xy).z;
		float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));

		occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	occlusion = 1.0 - (occlusion / kernelSize);
	FragColor = occlusion;
}