#version 450 core

in vec3 vColor;
in vec2 vTexCords;
out vec4 FragColor;

uniform float alphaVal;
uniform sampler2D ourTexture;

void main(){
    // FragColor = vec4(vColor, 1.0);
    // FragColor = vec4(vColor, alphaVal);
    FragColor = texture(ourTexture, vTexCords) * vec4(vColor, 1.0);
}