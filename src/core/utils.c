#include "utils.h"

#include <math.h>

float lerpf(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

vec2f_t lerpfv2(vec2f_t a, vec2f_t b, float t) {
    return (vec2f_t) { .x = lerpf(a.x, b.x, t), .y = lerpf(a.y, b.y, t) };
}

vec2f_t slerpfv2(vec2f_t a, vec2f_t b, float t) {
    return lerpfv2(cartesian2polarfv2(a), cartesian2polarfv2(b), t);
}

vec2f_t cartesian2polarfv2(vec2f_t vec) {
    return (vec2f_t) { hypotf(vec.x, vec.y), atan2f(vec.y, vec.x) };
}

color_t color_from_u32(uint32_t value) {
    return *(color_t*)&value;
}

uint32_t color_to_u32(color_t color) {
    return *(uint32_t*)&color;
}

color_t color_lerpf(color_t a, color_t b, float t) {
    float ar = a.r/255.0f;
    float ag = a.g/255.0f;
    float ab = a.b/255.0f;
    float aa = a.a/255.0f;

    float br = b.r/255.0f;
    float bg = b.g/255.0f;
    float bb = b.b/255.0f;
    float ba = b.a/255.0f;

    float rr = lerpf(ar, br, t);
    float rg = lerpf(ag, bg, t);
    float rb = lerpf(ab, bb, t);
    float ra = lerpf(aa, ba, t);

    return (color_t) { (uint8_t)(rr*255.0f), (uint8_t)(rg*255.0f), (uint8_t)(rb*255.0f), (uint8_t)(ra*255.0f) };
}