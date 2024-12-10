#ifndef _CUC_UTILS_H_
#define _CUC_UTILS_H_

#include <stdint.h>
#include <stdbool.h>

// TODO: namespace everything in cuc_utils or something else

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#define VECTOR2_FMT "(vec2f_t) { %f, %f }"
#define VECTOR2_ARG(__MACRO_INPUT_VEC__) (__MACRO_INPUT_VEC__).x, (__MACRO_INPUT_VEC__).y
#define VECTOR2_ZERO ((vec2f_t) { 0.0f, 0.0f })
#define VECTOR2_UP ((vec2f_t) { 0.0f, 1.0f })
#define VECTOR2_DOWN ((vec2f_t) { 0.0f, -1.0f })
#define VECTOR2_RIGHT ((vec2f_t) { 1.0f, 0.0f })
#define VECTOR2_LEFT ((vec2f_t) { -1.0f, 0.0f })
#define RECTANGLE_FMT "(rectf_t) { %f, %f, %f, %f }"
#define RECTANGLE_ARG(__MACRO_INPUT_RECTANGLE__) (__MACRO_INPUT_RECTANGLE__).x, (__MACRO_INPUT_RECTANGLE__).y, (__MACRO_INPUT_RECTANGLE__).width, (__MACRO_INPUT_RECTANGLE__).height
#define RECTANGLE_ZERO ((rectf_t) { 0.0f, 0.0f, 0.0f, 0.0f })

#define COLOR_LIGHTGRAY  (color_t) { 200, 200, 200, 255 }
#define COLOR_GRAY       (color_t) { 130, 130, 130, 255 }
#define COLOR_DARKGRAY   (color_t) { 80, 80, 80, 255 }
#define COLOR_YELLOW     (color_t) { 253, 249, 0, 255 }
#define COLOR_GOLD       (color_t) { 255, 203, 0, 255 }
#define COLOR_ORANGE     (color_t) { 255, 161, 0, 255 }
#define COLOR_PINK       (color_t) { 255, 109, 194, 255 }
#define COLOR_RED        (color_t) { 230, 41, 55, 255 }
#define COLOR_MAROON     (color_t) { 190, 33, 55, 255 }
#define COLOR_GREEN      (color_t) { 0, 228, 48, 255 }
#define COLOR_LIME       (color_t) { 0, 158, 47, 255 }
#define COLOR_DARKGREEN  (color_t) { 0, 117, 44, 255 }
#define COLOR_SKYBLUE    (color_t) { 102, 191, 255, 255 }
#define COLOR_BLUE       (color_t) { 0, 121, 241, 255 }
#define COLOR_DARKBLUE   (color_t) { 0, 82, 172, 255 }
#define COLOR_PURPLE     (color_t) { 200, 122, 255, 255 }
#define COLOR_VIOLET     (color_t) { 135, 60, 190, 255 }
#define COLOR_DARKPURPLE (color_t) { 112, 31, 126, 255 }
#define COLOR_BEIGE      (color_t) { 211, 176, 131, 255 }
#define COLOR_BROWN      (color_t) { 127, 106, 79, 255 }
#define COLOR_DARKBROWN  (color_t) { 76, 63, 47, 255 }
#define COLOR_WHITE      (color_t) { 255, 255, 255, 255 }
#define COLOR_BLACK      (color_t) { 0, 0, 0, 255 }
#define COLOR_BLANK      (color_t) { 0, 0, 0, 0 }
#define COLOR_MAGENTA    (color_t) { 255, 0, 255, 255 }
#define COLOR_RAYWHITE   (color_t) { 245, 245, 245, 255 }

typedef struct vec2f_t {
    float x;
    float y;
} vec2f_t;

typedef struct rectf_t {
    float x, y;
    float width, height;
} rectf_t;

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_t;

float deg_to_radf(float angle);
float rad_to_degf(float angle);
float clampf(float value, float min, float max);
float lerpf(float a, float b, float t);

vec2f_t vec2f_add(vec2f_t a, vec2f_t b);
vec2f_t vec2f_add_value(vec2f_t a, float b);
vec2f_t vec2f_sub(vec2f_t a, vec2f_t b);
vec2f_t vec2f_sub_value(vec2f_t a, float b);
vec2f_t vec2f_mult(vec2f_t a, vec2f_t b);
vec2f_t vec2f_mult_value(vec2f_t a, float b);
vec2f_t vec2f_div(vec2f_t a, vec2f_t b);
vec2f_t vec2f_div_value(vec2f_t a, float b);
vec2f_t vec2f_invert(vec2f_t vec);
vec2f_t vec2f_clamp(vec2f_t vec, vec2f_t min, vec2f_t max);
vec2f_t vec2f_clamp_value(vec2f_t vec, float min, float max);
bool vec2f_equal(vec2f_t a, vec2f_t b);
float vec2f_length(vec2f_t vec);
float vec2f_distance(vec2f_t a, vec2f_t b);
float vec2f_direction(vec2f_t a, vec2f_t b);
float vec2f_angle(vec2f_t vec);
vec2f_t vec2f_wrap_rect(vec2f_t vec, rectf_t rect);
vec2f_t vec2f_move(vec2f_t vec, float angle, float distance);
vec2f_t vec2f_move_towards(vec2f_t start, vec2f_t target, float distance);
vec2f_t vec2f_normalize(vec2f_t vec);
vec2f_t vec2f_lerp(vec2f_t a, vec2f_t b, float t);
vec2f_t vec2f_slerp(vec2f_t a, vec2f_t b, float t);
vec2f_t vec2f_cartesian2polar(vec2f_t vec);

color_t color_from_u32(uint32_t value);
uint32_t color_to_u32(color_t color);
color_t color_lerpf(color_t a, color_t b, float t);


#endif // _CUC_UTILS_H_