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