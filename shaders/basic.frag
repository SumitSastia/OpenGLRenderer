#version 450 core

in vec3 vColor;
in vec2 vTexCords;
out vec4 FragColor;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

uniform float balance;
uniform bool useTexture;

uniform float alphaVal;
uniform vec3 lightColor;

void main(){

    float ambientStength = 0.2;
    vec3 ambientLight = ambientStength * lightColor;

    vec3 resultColor = ambientLight * vColor;

    FragColor = vec4(resultColor, 1.0);

    // if(useTexture){
    //     FragColor = texture(ourTexture1, vTexCords) * vec4(vColor, 1.0);
    // }
    // else{
    //     FragColor = vec4(lightColor * vColor, 1.0);
    // }

    // FragColor = mix(texture(ourTexture2, vTexCords), texture(ourTexture1,vec2(vTexCords.x,vTexCords.y)), balance);
}