#ifndef _UTILS_H_
#define _UTILS_H_

#include <raylib/raylib.h>

#ifdef DEBUG_BUILD
#define ALLOW_LOGGING 1
#else
#define ALLOW_LOGGING 0
#endif // DEBUG_BUILD

typedef Rectangle rectangle_t;
typedef Vector2 vector2_t;

#define RECTANGLE_FMT "(rectangle_t) { %f, %f, %f, %f }"
#define RECTANGLE_ARG(__MACRO_INPUT_RECTANGLE__) __MACRO_INPUT_RECTANGLE__.x, __MACRO_INPUT_RECTANGLE__.y, __MACRO_INPUT_RECTANGLE__.width, __MACRO_INPUT_RECTANGLE__.height
#define VECTOR2_FMT "(vector2_t) { %f, %f }"
#define VECTOR2_ARG(__MACRO_INPUT_VEC__) __MACRO_INPUT_VEC__.x, __MACRO_INPUT_VEC__.y

float lerpf(float a, float b, float t);
vector2_t lerpfv2(vector2_t a, vector2_t b, float t);
vector2_t slerpfv2(vector2_t a, vector2_t b, float t);
vector2_t cartesian2polarfv2(vector2_t vec);

#endif // _UTILS_H_