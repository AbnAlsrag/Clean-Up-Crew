#include "cuc_utils.h"

#include <math.h>

float deg_to_radf(float angle) {
    return (angle * PI / 180.0f);
}

float rad_to_degf(float angle) {
    return (angle * 180.0f / PI);
}

float clampf(float value, float min, float max) {
    if (value <= min) {
        return min;
    }

    if (value >= max) {
        return max;
    }

    return value;
}

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
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

float vec2f_direction(vec2f_t a, vec2f_t b) {
    return rad_to_degf(atan2f(b.y - a.y, b.x - a.x));
}

float vec2f_angle(vec2f_t vec) {
    if (vec.x == 0.0f && vec.y == 0.0f) return 0.0f;
    return rad_to_degf(atan2f(vec.y, vec.x));
}

vec2f_t vec2f_wrap_rect(vec2f_t vec, rectf_t rect) {
    vec2f_t result = VECTOR2_ZERO;

    if (vec.x < rect.x) {
        result.x = (rect.x + rect.width) - (rect.x - vec.x);
    } else if (vec.x > rect.x + rect.width) {
        result.x = rect.x + (vec.x - (rect.x + rect.width));
    } else {
        result.x = vec.x;
    }

    if (vec.y < rect.y) {
        result.y = (rect.y + rect.height) - (rect.y - vec.y);
    } else if (vec.y > rect.y + rect.height) {
        result.y = rect.y + (vec.y - (rect.y + rect.height));
    } else {
        result.y = vec.y;
    }

    return result;
}

vec2f_t vec2f_move(vec2f_t vec, float angle, float distance) {
    vec2f_t result = VECTOR2_ZERO;
    result.x = vec.x + cosf(deg_to_radf(angle)) * distance;
    result.y = vec.y + sinf(deg_to_radf(angle)) * distance;
    return result;
}

vec2f_t vec2f_move_towards(vec2f_t start, vec2f_t target, float distance) {
    float dist = vec2f_distance(start, target);

    if (dist <= distance) {
        return target;
    }

    float angle = vec2f_direction(start, target);
    return vec2f_move(start, angle, distance);
}

vec2f_t vec2f_div_value(vec2f_t a, float b) {
    return (vec2f_t) { a.x / b, a.y / b };
}

vec2f_t vec2f_invert(vec2f_t vec) {
    return (vec2f_t) { -vec.x, -vec.y };
}

vec2f_t vec2f_clamp(vec2f_t vec, vec2f_t min, vec2f_t max) {
    vec2f_t result = { .x = clampf(vec.x, min.x, max.x), .y = clampf(vec.y, min.y, max.y) };
    return result;
}

vec2f_t vec2f_clamp_value(vec2f_t vec, float min, float max) {
    return (vec2f_t) { .x = clampf(vec.x, min, max), .y = clampf(vec.y, min, max) };
}

vec2f_t vec2f_normalize(vec2f_t vec) {
    vec2f_t result = VECTOR2_ZERO;

    float length = sqrtf((vec.x*vec.x) + (vec.y*vec.y));

    if (length == 0.0f) {
        return VECTOR2_ZERO;
    }

    result.x = vec.x/length;
    result.y = vec.y/length;

    return result;
}

vec2f_t vec2f_lerp(vec2f_t a, vec2f_t b, float t) {
    return (vec2f_t) { .x = lerpf(a.x, b.x, t), .y = lerpf(a.y, b.y, t) };
}

vec2f_t vec2f_slerp(vec2f_t a, vec2f_t b, float t) {
    return vec2f_lerp(vec2f_cartesian2polar(a), vec2f_cartesian2polar(b), t);
}

vec2f_t vec2f_cartesian2polar(vec2f_t vec) {
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