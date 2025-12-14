#version 450 core

in vec3 vColor;
in vec2 vTexCords;
out vec4 FragColor;

uniform float alphaVal;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float balance;

void main(){

    // FragColor = vec4(1.0,1.0,1.0,1.0);

    FragColor = vec4(vColor, 1.0);
    // FragColor = vec4(vColor, alphaVal);

    // FragColor = texture(ourTexture1, vTexCords) * vec4(vColor, 1.0);
    // FragColor = mix(texture(ourTexture2, vTexCords), texture(ourTexture1,vec2(vTexCords.x,vTexCords.y)), balance);
}