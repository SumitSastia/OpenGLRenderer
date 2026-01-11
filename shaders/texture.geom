#version 450 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 gPos[];
in vec3 gNormal[];
in vec2 gTextureCords[];

out vec3 vPos;
out vec3 vNormal;
out vec2 vTextureCords;

void main() {
	
	for (int i = 0; i < 3; i++) {
		
		vPos = gPos[i];
		vNormal = gNormal[i];
		vTextureCords = gTextureCords[i];

        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

	EndPrimitive();
}