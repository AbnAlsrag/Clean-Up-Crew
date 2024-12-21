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

typedef float angle_t;

typedef struct vec2f_t {
    float x;
    float y;
} vec2f_t;

typedef struct rectf_t {
    float x, y;
    float width, height;
} rectf_t;

typedef rectf_t aabb_t;

typedef struct obb_t {
    rectf_t rect;
    angle_t rotation;
} obb_t;

typedef struct obb_corners_t {
    vec2f_t corners[4];
} obb_corners_t;

// int x = sizeof(obb_t);

typedef struct circle_t {
    vec2f_t center;
    float radius;
} circle_t;

typedef struct circle_sector_t {
    vec2f_t center;
    float radius;
    float start_angle;
    float end_angle;
} circle_sector_t;

typedef struct ring_t {
    vec2f_t center;
    float outer_radius;
    float inner_radius;
    float start_angle;
    float end_angle;
} ring_t;

typedef struct triangle_t {
    vec2f_t point0;
    vec2f_t point1;
    vec2f_t point2;
} triangle_t;

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_t;

typedef struct timer_t {
    double start_time;
    double lifetime;
} timer_t;

timer_t start_timer(double lifetime);
bool is_timer_finished(timer_t timer);
double get_elapsed_time(timer_t timer);

float deg_to_radf(float angle);
float rad_to_degf(float angle);
float clampf(float value, float min, float max);
float lerpf(float a, float b, float t);
float inv_lerpf(float a, float b, float v);
float lerp_angle(float start_angle, float target_angle, float t);
float remapf(float imin, float imax, float omin, float omax, float v);

vec2f_t vec2f_add(vec2f_t a, vec2f_t b);
vec2f_t vec2f_add_value(vec2f_t a, float b);
vec2f_t vec2f_sub(vec2f_t a, vec2f_t b);
vec2f_t vec2f_sub_value(vec2f_t a, float b);
vec2f_t vec2f_mult(vec2f_t a, vec2f_t b);
vec2f_t vec2f_mult_value(vec2f_t a, float b);
vec2f_t vec2f_div(vec2f_t a, vec2f_t b);
vec2f_t vec2f_div_value(vec2f_t a, float b);
float vec2f_dot_product(vec2f_t a, vec2f_t b);
vec2f_t vec2f_opposite(vec2f_t vec);
vec2f_t vec2f_clamp(vec2f_t vec, vec2f_t min, vec2f_t max);
vec2f_t vec2f_clamp_value(vec2f_t vec, float min, float max);
bool vec2f_equal(vec2f_t a, vec2f_t b);
float vec2f_length(vec2f_t vec);
float vec2f_distance(vec2f_t a, vec2f_t b);
float vec2f_direction(vec2f_t a, vec2f_t b);
float vec2f_angle(vec2f_t vec);
vec2f_t vec2f_set_angle(vec2f_t vec, float angle);
vec2f_t vec2f_rotate(vec2f_t vec, float angle);
vec2f_t vec2f_wrap_rect(vec2f_t vec, rectf_t rect);
bool vec2f_out_of_rect(vec2f_t vec, rectf_t rect);
vec2f_t vec2f_move(vec2f_t vec, float angle, float distance);
vec2f_t vec2f_move_towards(vec2f_t start, vec2f_t target, float distance);
vec2f_t vec2f_normalize(vec2f_t vec);
vec2f_t vec2f_lerp(vec2f_t a, vec2f_t b, float v);
float vec2f_inv_lerp(vec2f_t a, vec2f_t b, vec2f_t value);
vec2f_t vec2f_slerp(vec2f_t a, vec2f_t b, float t);
vec2f_t vec2f_cartesian2polar(vec2f_t vec);
vec2f_t vec2f_polar2cartesian(vec2f_t vec);

rectf_t rectf_scale(rectf_t rect, float scalar);

color_t color_from_u32(uint32_t value);
uint32_t color_to_u32(color_t color);
color_t color_lerpf(color_t a, color_t b, float t);

bool check_collision_aabb_aabb(aabb_t a, aabb_t b);
bool check_collision_aabb_point(aabb_t aabb, vec2f_t point);
// TODO: test this
bool check_collision_aabb_circle(aabb_t aabb, circle_t circle);
bool check_collision_circle_circle(circle_t a, circle_t b);
bool check_collision_circle_point(circle_t circle, vec2f_t point);
bool check_collision_obb_obb(obb_t a, obb_t b);

aabb_t circle_bounding_box(circle_t circle);
aabb_t obb_bounding_box(obb_t obb);
obb_corners_t get_obb_corners(obb_t obb);

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Physics //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

typedef struct physics_object_t {
    vec2f_t pos;
    vec2f_t acceleration;
    vec2f_t velocity;
    float mass;
} physics_object_t;

void physics_apply_force(physics_object_t *obj, vec2f_t force);
void physics_update_obj(physics_object_t *obj, float dt);
void physics_set_obj_velocity(physics_object_t *obj, vec2f_t velocity);
void physics_clear_obj_forces(physics_object_t *obj);
void physics_kinetic_energy(physics_object_t *obj);
void physics_resolve_circle_collision(physics_object_t *a, physics_object_t *b, float restitution);

#endif // _CUC_UTILS_H_