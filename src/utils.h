#ifndef _UTILS_H_
#define _UTILS_H_

#include <raylib/raylib.h>

#ifdef DEBUG_BUILD
#define ALLOW_LOGGING 1
#else
#define ALLOW_LOGGING 0
#endif // DEBUG_BUILD

typedef Rectangle rectf_t;
typedef Vector2 vec2f_t;

#define RECTANGLE_FMT "(rectf_t) { %f, %f, %f, %f }"
#define RECTANGLE_ARG(__MACRO_INPUT_RECTANGLE__) (__MACRO_INPUT_RECTANGLE__).x, (__MACRO_INPUT_RECTANGLE__).y, (__MACRO_INPUT_RECTANGLE__).width, (__MACRO_INPUT_RECTANGLE__).height
#define VECTOR2_FMT "(vec2f_t) { %f, %f }"
#define VECTOR2_ARG(__MACRO_INPUT_VEC__) (__MACRO_INPUT_VEC__).x, (__MACRO_INPUT_VEC__).y

float lerpf(float a, float b, float t);
vec2f_t lerpfv2(vec2f_t a, vec2f_t b, float t);
vec2f_t slerpfv2(vec2f_t a, vec2f_t b, float t);
vec2f_t cartesian2polarfv2(vec2f_t vec);

#endif // _UTILS_H_