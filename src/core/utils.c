#include "utils.h"

#include <math.h>

float lerpf(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

vec2f_t vec2f_add(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x + b.x, a.y + b.y };
}

vec2f_t vec2f_add_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x + b, a.y + b };
}

vec2f_t vec2f_sub(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x - b.x, a.y - b.y };
}

vec2f_t vec2f_sub_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x - b, a.y - b };
}

vec2f_t vec2f_mult(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x * b.x, a.y * b.y };
}

vec2f_t vec2f_mult_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x * b, a.y * b };
}

vec2f_t vec2f_div(vec2f_t a, vec2f_t b) {
    return (vec2f_t) { a.x / b.x, a.y / b.y };
}

bool vec2f_equal(vec2f_t a, vec2f_t b) {
    bool result = ((fabsf(a.x - b.x)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(a.x), fabsf(b.x))))) &&
                ((fabsf(a.y - b.y)) <= (EPSILON*fmaxf(1.0f, fmaxf(fabsf(a.y), fabsf(b.y)))));

    return result;
}

float vec2f_length(vec2f_t vec) {
    return sqrtf((vec.x*vec.x) + (vec.y*vec.y));
}

float vec2f_distance(vec2f_t a, vec2f_t b) {
    return sqrtf((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

vec2f_t vec2f_div_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x / b, a.y / b };
}

vec2f_t vec2f_normalize(vec2f_t vec) {
    vec2f_t result = VECTOR2_ZERO;

    float length = sqrtf((vec.x*vec.x) + (vec.y*vec.y));

    if (length > 0) {
        float ilength = 1.0f/length;
        result.x = vec.x*ilength;
        result.y = vec.y*ilength;
    }

    return result;
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