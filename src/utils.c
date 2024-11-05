#include "utils.h"

#include <math.h>

float lerpf(float a, float b, float t) {
    return (1 - t) * a + t * b;
}

vector2_t lerpfv2(vector2_t a, vector2_t b, float t) {
    return (vector2_t) { .x = lerpf(a.x, b.x, t), .y = lerpf(a.y, b.y, t) };
}

vector2_t slerpfv2(vector2_t a, vector2_t b, float t) {
    return lerpfv2(cartesian2polarfv2(a), cartesian2polarfv2(b), t);
}

vector2_t cartesian2polarfv2(vector2_t vec) {
    return (vector2_t) { hypotf(vec.x, vec.y), atan2f(vec.y, vec.x) };
}