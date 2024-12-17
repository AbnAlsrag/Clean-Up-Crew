// ˈrərə

#ifndef _RRR_H_
#define _RRR_H_

#ifndef RRR_API
#define RRR_API
#endif

#include <math.h>

#ifndef RRR_CUSTOM_TYPES
#include <stdint.h>
#include <stdbool.h>

#define RRR_TRUE 1
#define RRR_FALSE 0
typedef bool rrr_bool_t;

typedef uint8_t rrr_u8_t;
typedef int8_t rrr_i8_t;
typedef uint16_t rrr_u16_t;
typedef int16_t rrr_i16_t;
typedef uint32_t rrr_u32_t;
typedef int32_t rrr_i32_t;
typedef uint64_t rrr_u64_t;
typedef int64_t rrr_i64_t;
typedef size_t rrr_usize_t;
typedef ssize_t rrr_isize_t;

typedef float rrr_f32_t;
typedef double rrr_f64_t;
#endif

#define RRR_COLOR_LIGHTGRAY  (rrr_color_t) { 200, 200, 200, 255 }
#define RRR_COLOR_GRAY       (rrr_color_t) { 130, 130, 130, 255 }
#define RRR_COLOR_DARKGRAY   (rrr_color_t) { 80, 80, 80, 255 }
#define RRR_COLOR_YELLOW     (rrr_color_t) { 253, 249, 0, 255 }
#define RRR_COLOR_GOLD       (rrr_color_t) { 255, 203, 0, 255 }
#define RRR_COLOR_ORANGE     (rrr_color_t) { 255, 161, 0, 255 }
#define RRR_COLOR_PINK       (rrr_color_t) { 255, 109, 194, 255 }
#define RRR_COLOR_RED        (rrr_color_t) { 230, 41, 55, 255 }
#define RRR_COLOR_MAROON     (rrr_color_t) { 190, 33, 55, 255 }
#define RRR_COLOR_GREEN      (rrr_color_t) { 0, 228, 48, 255 }
#define RRR_COLOR_LIME       (rrr_color_t) { 0, 158, 47, 255 }
#define RRR_COLOR_DARKGREEN  (rrr_color_t) { 0, 117, 44, 255 }
#define RRR_COLOR_SKYBLUE    (rrr_color_t) { 102, 191, 255, 255 }
#define RRR_COLOR_BLUE       (rrr_color_t) { 0, 121, 241, 255 }
#define RRR_COLOR_DARKBLUE   (rrr_color_t) { 0, 82, 172, 255 }
#define RRR_COLOR_PURPLE     (rrr_color_t) { 200, 122, 255, 255 }
#define RRR_COLOR_VIOLET     (rrr_color_t) { 135, 60, 190, 255 }
#define RRR_COLOR_DARKPURPLE (rrr_color_t) { 112, 31, 126, 255 }
#define RRR_COLOR_BEIGE      (rrr_color_t) { 211, 176, 131, 255 }
#define RRR_COLOR_BROWN      (rrr_color_t) { 127, 106, 79, 255 }
#define RRR_COLOR_DARKBROWN  (rrr_color_t) { 76, 63, 47, 255 }
#define RRR_COLOR_WHITE      (rrr_color_t) { 255, 255, 255, 255 }
#define RRR_COLOR_BLACK      (rrr_color_t) { 0, 0, 0, 255 }
#define RRR_COLOR_BLANK      (rrr_color_t) { 0, 0, 0, 0 }
#define RRR_COLOR_MAGENTA    (rrr_color_t) { 255, 0, 255, 255 }
#define RRR_COLOR_RAYWHITE   (rrr_color_t) { 245, 245, 245, 255 }

#define RRR_VEC2_ZERO (rrr_vec2_t) { 0, 0 }
#define RRR_RECT_ZERO (rrr_rect_t) { 0, 0, 0, 0 }

typedef void (*rrr_painter_job_t)(void);
typedef void (*rrr_painter_run_proc_t)(void *data, rrr_painter_job_t job);

typedef struct rrr_painter_t {
    void *data;
    rrr_painter_run_proc_t run_proc;
} rrr_painter_t;

typedef struct rrr_color_t {
    rrr_u8_t r;
    rrr_u8_t g;
    rrr_u8_t b;
    rrr_u8_t a;
} rrr_color_t;

typedef rrr_color_t (*rrr_pixel_shader_t)(void);

typedef struct rrr_shader_program_t {
    rrr_pixel_shader_t pixel_shader;
} rrr_shader_program_t;

typedef struct rrr_context_t {
    rrr_bool_t unused;
} rrr_context_t;

typedef struct rrr_image_t {
    rrr_color_t *pixels;
    rrr_u32_t width;
    rrr_u32_t height;
    rrr_u32_t stride;
} rrr_image_t;

typedef struct rrr_canvas_t {
    rrr_image_t image;
} rrr_canvas_t;

typedef struct rrr_vec2f_t {
    rrr_f32_t x;
    rrr_f32_t y;
} rrr_vec2f_t;

typedef struct rrr_rect_t {
    rrr_i32_t x;
    rrr_i32_t y;
    rrr_u32_t width;
    rrr_u32_t height;
} rrr_rect_t;

typedef struct rrr_circle_t {
    rrr_vec2f_t center;
    rrr_u32_t radius;
} rrr_circle_t;

#define RRR_IMAGE_PIXEL(_MACRO_INPUT_IMAGE_, _MACRO_INPUT_X_, _MACRO_INPUT_Y_) (_MACRO_INPUT_IMAGE_).pixels[(_MACRO_INPUT_Y_)*(_MACRO_INPUT_IMAGE_).stride + _MACRO_INPUT_X_]

RRR_API rrr_f32_t rrr_vec2f_distance(rrr_vec2f_t a, rrr_vec2f_t b);

RRR_API rrr_context_t rrr_create_context(void);
RRR_API void rrr_make_context_current(rrr_context_t context);
RRR_API rrr_image_t rrr_create_image(rrr_color_t *pixels, rrr_u32_t width, rrr_u32_t height, rrr_u32_t stride);
RRR_API rrr_bool_t rrr_is_image_valid(rrr_image_t image);
RRR_API rrr_canvas_t rrr_create_canvas(rrr_color_t *pixels, rrr_u32_t width, rrr_u32_t height, rrr_u32_t stride);
RRR_API rrr_canvas_t rrr_create_canvas_from_image(rrr_image_t image);
RRR_API rrr_rect_t rrr_canvas_get_normalized_rect(rrr_canvas_t canvas, rrr_rect_t rect);
RRR_API void rrr_canvas_fill(rrr_canvas_t canvas, rrr_color_t color);
RRR_API void rrr_canvas_draw_rect(rrr_canvas_t canvas, rrr_rect_t rect, rrr_color_t color);
RRR_API void rrr_canvas_draw_circle(rrr_canvas_t canvas, rrr_circle_t circle, rrr_color_t color);

#ifdef RRR_IMPLEMENTATION
#endif // RRR_IMPLEMENTATION

static rrr_context_t _rrr_internal_current_context_ = {0};

RRR_API rrr_f32_t rrr_vec2f_distance(rrr_vec2f_t a, rrr_vec2f_t b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return sqrtf(dx * dx + dy * dy);
}

RRR_API rrr_context_t rrr_create_context(void) {
    rrr_context_t context = {0};

    return context;
}

RRR_API void rrr_make_context_current(rrr_context_t context) {
    _rrr_internal_current_context_ = context;
}

RRR_API rrr_image_t rrr_create_image(rrr_color_t *pixels, rrr_u32_t width, rrr_u32_t height, rrr_u32_t stride) {
    rrr_image_t image = {
        .pixels = pixels,
        .width = width,
        .height = height,
        .stride = stride,
    };

    return image;
}

RRR_API rrr_bool_t rrr_is_image_valid(rrr_image_t image) {
    return (image.pixels != NULL);
}

RRR_API rrr_canvas_t rrr_create_canvas(rrr_color_t *pixels, rrr_u32_t width, rrr_u32_t height, rrr_u32_t stride) {
    rrr_canvas_t canvas = {
        .image = rrr_create_image(pixels, width, height, stride),
    };

    return canvas;
}

RRR_API rrr_canvas_t rrr_create_canvas_from_image(rrr_image_t image) {
    rrr_canvas_t canvas = {
        .image = image,
    };

    return canvas;
}

RRR_API rrr_rect_t rrr_canvas_get_normalized_rect(rrr_canvas_t canvas, rrr_rect_t rect) {
    rrr_rect_t result = rect;

    // Clamp x and adjust width
    if (result.x < 0) {
        result.width += result.x;
        result.x = 0;
    }
    if (result.x >= canvas.image.width) {
        result.x = canvas.image.width;
        result.width = 0;
    }

    // Clamp y and adjust height
    if (result.y < 0) {
        result.height += result.y;
        result.y = 0;
    }
    if (result.y >= canvas.image.height) {
        result.y = canvas.image.height;
        result.height = 0;
    }

    // Adjust width to stay within bounds
    if (result.x + result.width > canvas.image.width) {
        result.width = canvas.image.width - result.x;
    }
    if (result.width < 0) {
        result.width = 0;
    }

    // Adjust height to stay within bounds
    if (result.y + result.height > canvas.image.height) {
        result.height = canvas.image.height - result.y;
    }
    if (result.height < 0) {
        result.height = 0;
    }

    return result;
}

RRR_API void rrr_canvas_fill(rrr_canvas_t canvas, rrr_color_t color) {
    if (!rrr_is_image_valid(canvas.image)) {
        return;
    }

    for (rrr_usize_t y = 0; y < canvas.image.height; y++) {
        for (rrr_usize_t x = 0; x < canvas.image.width; x++) {
            RRR_IMAGE_PIXEL(canvas.image, x, y) = color;
        }
    }
}

RRR_API void rrr_canvas_draw_rect(rrr_canvas_t canvas, rrr_rect_t rect, rrr_color_t color) {
    if (!rrr_is_image_valid(canvas.image)) {
        return;
    }

    rrr_rect_t draw_rect = rrr_canvas_get_normalized_rect(canvas, rect);
    // rrr_rect_t draw_rect = rect;

    for (rrr_usize_t y = draw_rect.y; y < draw_rect.y+draw_rect.height; y++) {
        for (rrr_usize_t x = draw_rect.x; x < draw_rect.x+draw_rect.width; x++) {
            RRR_IMAGE_PIXEL(canvas.image, x, y) = color;
        }
    }
}

RRR_API void rrr_canvas_draw_circle(rrr_canvas_t canvas, rrr_circle_t circle, rrr_color_t color) {
    if (!rrr_is_image_valid(canvas.image)) {
        return;
    }

    rrr_rect_t bounding_rect = (rrr_rect_t) { circle.center.x-circle.radius, circle.center.y-circle.radius, circle.radius*2, circle.radius*2 };
    rrr_rect_t draw_rect = rrr_canvas_get_normalized_rect(canvas, bounding_rect);

    for (rrr_usize_t y = draw_rect.y; y < draw_rect.y+draw_rect.height; y++) {
        for (rrr_usize_t x = draw_rect.x; x < draw_rect.x+draw_rect.width; x++) {
            rrr_f32_t dx = x - circle.center.x;
            rrr_f32_t dy = y - circle.center.y;
            rrr_f32_t distance_squared = dx * dx + dy * dy;

            if (distance_squared <= circle.radius * circle.radius) {
                RRR_IMAGE_PIXEL(canvas.image, x, y) = color;
            }
        }
    }
}

#endif // _RRR_H_