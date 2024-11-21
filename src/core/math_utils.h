#ifndef _MATH_UTILS_H_
#define _MATH_UTILS_H_

#include <stdint.h>

typedef struct vec2f_t {
    float x;
    float y;
} vec2f_t;

typedef struct rectf_t {
    float x, y;
    float width, height;
} rectf_t;

#define VECTOR2_FMT "(vec2f_t) { %f, %f }"
#define VECTOR2_ARG(__MACRO_INPUT_VEC__) (__MACRO_INPUT_VEC__).x, (__MACRO_INPUT_VEC__).y
#define VECTOR2_ZERO (vec2f_t) { 0, 0 }
#define RECTANGLE_FMT "(rectf_t) { %f, %f, %f, %f }"
#define RECTANGLE_ARG(__MACRO_INPUT_RECTANGLE__) (__MACRO_INPUT_RECTANGLE__).x, (__MACRO_INPUT_RECTANGLE__).y, (__MACRO_INPUT_RECTANGLE__).width, (__MACRO_INPUT_RECTANGLE__).height

float lerpf(float a, float b, float t);
vec2f_t lerpfv2(vec2f_t a, vec2f_t b, float t);
vec2f_t slerpfv2(vec2f_t a, vec2f_t b, float t);
vec2f_t cartesian2polarfv2(vec2f_t vec);

#endif // _MATH_H_