#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "core/math_utils.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum config_flags_t {
    CONFIG_FLAG_VSYNC = 0x00000001,
    CONFIG_FLAG_FULLSCREEN = 0x00000002,
    CONFIG_FLAG_BORDERLESS_WINDOWED = 0x00000004,
} config_flags_t;

typedef struct platform_config_t {
    uint32_t width;
    uint32_t height;
    char *title;
    uint32_t target_fps;
    config_flags_t config_flags;
} platform_config_t;

typedef struct viewport_t {
    uint32_t x, y;
    uint32_t width, height;
} viewport_t;

typedef struct camera_t {
    vec2f_t offset;
    vec2f_t target;
    float rotation;
    float zoom;
} camera_t;

typedef enum blend_mode_t {
    BLEND_MODE_ALPHA = 0,
    BLEND_MODE_ADDITIVE,
    BLEND_MODE_MULTIPLIED,
    BLEND_MODE_ADD_COLORS,
    BLEND_MODE_SUBTRACT_COLORS,
    BLEND_MODE_ALPHA_PREMULTIPLY,
} blend_mode_t;

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

typedef struct color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_t;

typedef struct texture_t {
    uint32_t id;
    uint32_t width;
    uint32_t height;
} texture_t;

void platform_init(platform_config_t config);
void platform_reconfig(platform_config_t config);
void platform_deinit(void);
bool platform_running(void);
bool platform_was_window_resized(void);
vec2f_t get_window_size(void);

float set_master_volume(float volume);
float get_master_volume(void);

void clear_background(color_t color);
void begin_drawing(void);
void end_drawing(void);
void begin_viewport(viewport_t viewport);
void end_viewport(void);
void begin_camera(camera_t camera);
void end_camera(void);
void begin_blend_mode(blend_mode_t mode);
void end_blend_mode(void);
vec2f_t get_world_to_screen(camera_t camera, vec2f_t pos);
vec2f_t get_screen_to_world(camera_t camera, vec2f_t pos);

texture_t load_texture(char *file_path);
bool is_texture_valid(texture_t texture);
void unload_texture(texture_t texture);

void draw_line(vec2f_t start, vec2f_t end, float thickness, color_t color);
void draw_triangle(vec2f_t point0, vec2f_t point1, vec2f_t point2, color_t color);
void draw_rect(rectf_t rect, vec2f_t origin, float rotation, color_t color);
void draw_circle(vec2f_t center, float radius, color_t color);
void draw_circle_sector(vec2f_t center, float radius, float start_angle, float end_angle, color_t color);
void draw_texture(texture_t texture, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint);

float get_frame_time(void);
double get_time(void);
uint32_t get_fps(void);
static inline color_t get_color(uint32_t value) { return *(color_t*)&value; }

void open_url(char *url);

#endif // _PLATFORM_H_