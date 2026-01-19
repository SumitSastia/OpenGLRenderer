
float init_shadow(vec4 lightSpacePos, sampler2D depthMap, vec3 normal, vec3 lightDirection) {

    if (lightSpacePos.w <= 0.0) {
        return 0.0;
    }

    vec3 shadowCords = lightSpacePos.xyz / lightSpacePos.w;
    shadowCords = shadowCords * 0.5 + 0.5;

    if (shadowCords.x < 0.0 || shadowCords.x > 1.0 ||
        shadowCords.y < 0.0 || shadowCords.y > 1.0 ||
        shadowCords.z < 0.0 || shadowCords.z > 1.0) {
        return 0.0;
    }

    float nearest_depth = texture(depthMap, shadowCords.xy).r;
    float shadow = 0.0;

    vec3 final_normal = normalize(normal);

    float bias = max(
        0.005 * (1.0 - dot(final_normal, lightDirection)),
        0.0005
    );

    // float bias = 0.0025;

    if (shadowCords.z - bias > nearest_depth) {
        shadow = 1.0;
    }

    return shadow;
}