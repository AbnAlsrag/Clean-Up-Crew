#ifndef _IMGUI_H_
#define _IMGUI_H_

#ifndef IMGUI_API
#define IMGUI_API
#endif // IMGUI_API

#include <stdint.h>
#include <stdbool.h>

#define IMGUI_COLOR_LIGHTGRAY  (imgui_color_t){ 200, 200, 200, 255 }   // Light Gray
#define IMGUI_COLOR_GRAY       (imgui_color_t){ 130, 130, 130, 255 }   // Gray
#define IMGUI_COLOR_DARKGRAY   (imgui_color_t){ 80, 80, 80, 255 }      // Dark Gray
#define IMGUI_COLOR_YELLOW     (imgui_color_t){ 253, 249, 0, 255 }     // Yellow
#define IMGUI_COLOR_GOLD       (imgui_color_t){ 255, 203, 0, 255 }     // Gold
#define IMGUI_COLOR_ORANGE     (imgui_color_t){ 255, 161, 0, 255 }     // Orange
#define IMGUI_COLOR_PINK       (imgui_color_t){ 255, 109, 194, 255 }   // Pink
#define IMGUI_COLOR_RED        (imgui_color_t){ 230, 41, 55, 255 }     // Red
#define IMGUI_COLOR_MAROON     (imgui_color_t){ 190, 33, 55, 255 }     // Maroon
#define IMGUI_COLOR_GREEN      (imgui_color_t){ 0, 228, 48, 255 }      // Green
#define IMGUI_COLOR_LIME       (imgui_color_t){ 0, 158, 47, 255 }      // Lime
#define IMGUI_COLOR_DARKGREEN  (imgui_color_t){ 0, 117, 44, 255 }      // Dark Green
#define IMGUI_COLOR_SKYBLUE    (imgui_color_t){ 102, 191, 255, 255 }   // Sky Blue
#define IMGUI_COLOR_BLUE       (imgui_color_t){ 0, 121, 241, 255 }     // Blue
#define IMGUI_COLOR_DARKBLUE   (imgui_color_t){ 0, 82, 172, 255 }      // Dark Blue
#define IMGUI_COLOR_PURPLE     (imgui_color_t){ 200, 122, 255, 255 }   // Purple
#define IMGUI_COLOR_VIOLET     (imgui_color_t){ 135, 60, 190, 255 }    // Violet
#define IMGUI_COLOR_DARKPURPLE (imgui_color_t){ 112, 31, 126, 255 }    // Dark Purple
#define IMGUI_COLOR_BEIGE      (imgui_color_t){ 211, 176, 131, 255 }   // Beige
#define IMGUI_COLOR_BROWN      (imgui_color_t){ 127, 106, 79, 255 }    // Brown
#define IMGUI_COLOR_DARKBROWN  (imgui_color_t){ 76, 63, 47, 255 }      // Dark Brown
#define IMGUI_COLOR_WHITE      (imgui_color_t){ 255, 255, 255, 255 }   // White
#define IMGUI_COLOR_BLACK      (imgui_color_t){ 0, 0, 0, 255 }         // Black
#define IMGUI_COLOR_BLANK      (imgui_color_t){ 0, 0, 0, 0 }           // Blank (Transparent)
#define IMGUI_COLOR_MAGENTA    (imgui_color_t){ 255, 0, 255, 255 }     // Magenta
#define IMGUI_COLOR_RAYWHITE   (imgui_color_t){ 245, 245, 245, 255 }   // My own White (raylib logo)

typedef struct imgui_color_t {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} imgui_color_t;

typedef struct imgui_vec2_t {
    float x, y;
} imgui_vec2_t;

typedef struct imgui_rect_t {
    float x, y;
    float width, height;
} imgui_rect_t;

typedef struct imgui_camera_t {
    imgui_vec2_t offset;
    imgui_vec2_t target;
    float rotation;
    float zoom;
} imgui_camera_t;

#define imgui_String(__MACRO_INPUT_CSTR__) (imgui_string_t) { __MACRO_INPUT_CSTR__, sizeof(__MACRO_INPUT_CSTR__)-1 }

typedef struct imgui_string_t {
    char *characters;
    size_t len;
} imgui_string_t;

typedef struct imgui_text_buffer_t {
    imgui_string_t text;
    size_t size;
} imgui_text_buffer_t;

#define IMGUI_UNLIMITED ((size_t)-1)

IMGUI_API imgui_camera_t imgui_Camera2D_to_camera(Camera2D camera);
IMGUI_API imgui_vec2_t imgui_screen_to_world_coordinate(imgui_vec2_t vec);
IMGUI_API imgui_vec2_t imgui_get_mouse_world_coordinate(void);
IMGUI_API void imgui_begin_camera_mode(imgui_camera_t camera);
IMGUI_API void imgui_end_camera_mode(void);
IMGUI_API void imgui_begin_tooltip(imgui_string_t text);
IMGUI_API void imgui_end_tooltip(void);
IMGUI_API void imgui_rect(imgui_rect_t rect, float border_width, imgui_color_t color, imgui_color_t border_color);
IMGUI_API void imgui_label(imgui_rect_t rect, const imgui_string_t text, imgui_color_t color);
IMGUI_API bool imgui_button(imgui_rect_t rect, const imgui_string_t text);
IMGUI_API bool imgui_repeat_button(imgui_rect_t rect, const imgui_string_t text);
IMGUI_API bool imgui_toggle_button(imgui_rect_t rect, const imgui_string_t text, bool toggle);
IMGUI_API size_t imgui_tab_menu(imgui_rect_t rect, const imgui_string_t *tabs, size_t tab_count, size_t current_tab);
IMGUI_API size_t imgui_contex_menu(const imgui_string_t *items, size_t item_count);
IMGUI_API float imgui_slider(imgui_rect_t rect, const imgui_string_t left_text, const imgui_string_t right_text, float value, float min, float max);
IMGUI_API void imgui_text_box(imgui_rect_t rect, imgui_text_buffer_t text_buffer, size_t max_size);
IMGUI_API bool imgui_check_collision_rects(imgui_rect_t rect1, imgui_rect_t rect2);
IMGUI_API bool imgui_check_collision_rect_point(imgui_vec2_t point, imgui_rect_t rect);

#define IMGUI_IMPLEMENTATION
#ifdef IMGUI_IMPLEMENTATION

#include <raylib/raylib.h>

#define IMGUI_INTERNAL_DEFAULT_FONT_SIZE 28
#define IMGUI_INTERNAL_DEFAULT_TAB_WIDTH 160
#define IMGUI_INTERNAL_DEFAULT_CONTEXT_MENU_WIDTH 150
#define IMGUI_INTERNAL_DEFAULT_CONTEX_ITEM_HEIGHT 50
#define IMGUI_INTERNAL_DEFAULT_MAX_CONTEX_ITEMS 15

typedef struct imgui_internal_data_t {
    imgui_camera_t camera;
    bool is_camera_active;
    imgui_string_t tooltip_text;
    bool is_tooltip_active;
    Font custom_font;
    bool is_custom_font;
    bool is_slider_dragged;
    bool is_contex_menu_open;
    imgui_vec2_t contex_menu_pos;
    imgui_string_t contex_items[IMGUI_INTERNAL_DEFAULT_MAX_CONTEX_ITEMS];
    size_t contex_item_count;
    imgui_rect_t current_slider_rect;
    char tmp_buffer[520];
} imgui_internal_data_t;

static imgui_internal_data_t imgui_internal_data = {0};

IMGUI_API Vector2 imgui_internal_vec2_to_Vector2(imgui_vec2_t vector) {
    Vector2 result = { vector.x, vector.y };

    return result;
}

IMGUI_API imgui_vec2_t imgui_internal_Vector2_to_vec2(Vector2 vector) {
    imgui_vec2_t result = { vector.x, vector.y };

    return result;
}

IMGUI_API bool imgui_internal_rect_is_equal(imgui_rect_t a, imgui_rect_t b) {
    return (a.x == b.x && a.y == b.y && a.width == b.width && a.height == b.height);
}

IMGUI_API Rectangle imgui_internal_rect_to_Rectangle(imgui_rect_t rect) {
    Rectangle result = { rect.x, rect.y, rect.width, rect.height };

    return result;
}

IMGUI_API imgui_rect_t imgui_internal_Rectangle_to_rect(Rectangle rect) {
    imgui_rect_t result = { rect.x, rect.y, rect.width, rect.height };

    return result;
}

IMGUI_API Color imgui_internal_color_to_Color(imgui_color_t color) {
    Color result = { color.r, color.g, color.b, color.a };

    return result;
}

IMGUI_API imgui_color_t imgui_internal_Color_to_color(Color color) {
    imgui_color_t result = { color.r, color.g, color.b, color.a };

    return result;
}

IMGUI_API Camera2D imgui_internal_camera_Camera2D(imgui_camera_t camera) {
    Camera2D result = {
            .offset = imgui_internal_vec2_to_Vector2(imgui_internal_data.camera.offset),
            .target = imgui_internal_vec2_to_Vector2(imgui_internal_data.camera.target),
            .rotation = imgui_internal_data.camera.rotation,
            .zoom = imgui_internal_data.camera.zoom };
    
    return result;
}

IMGUI_API imgui_vec2_t imgui_internal_measure_text(imgui_string_t text, float font_size) {
    imgui_vec2_t result = {0};

    if (imgui_internal_data.is_custom_font) {
        result = imgui_internal_Vector2_to_vec2(MeasureTextEx(imgui_internal_data.custom_font, text.characters, font_size, 1.0f));
    } else {
        result = imgui_internal_Vector2_to_vec2(MeasureTextEx(GetFontDefault(), text.characters, font_size, 1.0f));
    }

    return result;
}

IMGUI_API void imgui_internal_draw_tooltip(void) {
    if (!imgui_internal_data.is_tooltip_active) {
        return;
    }

    Rectangle rect = {0};
    imgui_vec2_t mouse_pos = imgui_get_mouse_world_coordinate();
    imgui_vec2_t text_size = imgui_internal_measure_text(imgui_internal_data.tooltip_text, IMGUI_INTERNAL_DEFAULT_FONT_SIZE);

    rect.x = mouse_pos.x;
    rect.y = mouse_pos.y - text_size.y;
    rect.width = text_size.x;
    rect.height = text_size.y;
    
    imgui_label(imgui_internal_Rectangle_to_rect(rect), imgui_internal_data.tooltip_text, IMGUI_COLOR_LIME);
}

IMGUI_API imgui_camera_t imgui_Camera2D_to_camera(Camera2D camera) {
    imgui_camera_t result = {
        .offset = imgui_internal_Vector2_to_vec2(camera.offset),
        .target = imgui_internal_Vector2_to_vec2(camera.target),
        .rotation = camera.rotation,
        .zoom = camera.zoom };
    
    return result;
}

IMGUI_API imgui_vec2_t imgui_screen_to_world_coordinate(imgui_vec2_t vec) {
    imgui_vec2_t result = {0};

    if (imgui_internal_data.is_camera_active) {
        Camera2D camera = imgui_internal_camera_Camera2D(imgui_internal_data.camera);
        result = imgui_internal_Vector2_to_vec2(GetScreenToWorld2D(imgui_internal_vec2_to_Vector2(vec), camera));
    } else {
        result = vec;
    }

    return result;
}

IMGUI_API imgui_vec2_t imgui_get_mouse_world_coordinate(void) {
    imgui_vec2_t mouse_pos = imgui_internal_Vector2_to_vec2(GetMousePosition());
    imgui_vec2_t result = imgui_screen_to_world_coordinate(mouse_pos);

    return result;
}

IMGUI_API void imgui_begin_camera_mode(imgui_camera_t camera) {
    imgui_internal_data.camera = camera;
    imgui_internal_data.is_camera_active = true;
}

IMGUI_API void imgui_end_camera_mode(void) {
    imgui_internal_data.is_camera_active = false;
}

IMGUI_API void imgui_begin_tooltip(imgui_string_t text) {
    imgui_internal_data.is_tooltip_active = true;
    imgui_internal_data.tooltip_text = text;
}

IMGUI_API void imgui_end_tooltip(void) {
    imgui_internal_data.is_tooltip_active = false;
}

IMGUI_API void imgui_rect(imgui_rect_t rect, float border_width, imgui_color_t color, imgui_color_t border_color) {
    Color rect_color = imgui_internal_color_to_Color(color);
    Color border_rect_color = imgui_internal_color_to_Color(border_color);
    
    if (color.a == 255) {
        if (border_color.a > 0 && border_width > 0.0f) {
            DrawRectangle(rect.x - border_width, rect.y - border_width, rect.width + border_width*2, rect.height + border_width*2, border_rect_color);
        }
    } else if (border_color.a > 0 && border_width > 0.0f) {
        DrawRectangle(rect.x - border_width, rect.y - border_width, rect.width + border_width, border_width, border_rect_color);
        DrawRectangle(rect.x - border_width, rect.y + rect.height, rect.width + border_width, border_width, border_rect_color);
        DrawRectangle(rect.x - border_width, rect.y, border_width, rect.height + border_width, border_rect_color);
        DrawRectangle(rect.x + rect.width, rect.y, border_width, rect.height, border_rect_color);
    }

    if (color.a > 0) {

        DrawRectangleRec(imgui_internal_rect_to_Rectangle(rect), rect_color);
    }
}

// NOTE: this fuction would fail if the data in the text doesn't have a zero byte at the end :>
IMGUI_API void imgui_label(imgui_rect_t rect, const imgui_string_t text, imgui_color_t color) {
    float font_size = IMGUI_INTERNAL_DEFAULT_FONT_SIZE;
    imgui_vec2_t text_size;
    
    do {
        text_size = imgui_internal_measure_text(text, font_size);

        if (text_size.x > rect.width || text_size.y > rect.height) {
            font_size -= 1.0f;
        } else {
            break;
        }
    } while (font_size > 1.0f);

    float text_x = rect.x + (rect.width - text_size.x) / 2;
    float text_y = rect.y + (rect.height - text_size.y) / 2;

    if (imgui_internal_data.is_custom_font) {
        DrawTextEx(imgui_internal_data.custom_font, text.characters, (Vector2) { text_x, text_y }, font_size, 1.0f, BLACK);
    } else {
        DrawTextEx(GetFontDefault(), text.characters, (Vector2) { text_x, text_y }, font_size, 1.0f, imgui_internal_color_to_Color(color));
    }
}

IMGUI_API bool imgui_button(imgui_rect_t rect, const imgui_string_t text) {
    bool result = false;
    int state = 0;
    bool draw_tooltip = false;

    if (imgui_check_collision_rect_point(imgui_get_mouse_world_coordinate(), rect)) {
        state = 1;
        draw_tooltip = true;        

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state = 2;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result = true;
        }
    }

    if (state == 0) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, LIGHTGRAY);
    } else if (state == 1) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, DARKGRAY);
    } else if (state == 2) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
    }

    imgui_label(rect, text, IMGUI_COLOR_BLACK);

    if (draw_tooltip) {
        imgui_internal_draw_tooltip();
    }

    return result;
}

IMGUI_API bool imgui_repeat_button(imgui_rect_t rect, const imgui_string_t text) {
    bool result = false;
    int state = 0;
    bool draw_tooltip = false;

    if (imgui_check_collision_rect_point(imgui_get_mouse_world_coordinate(), rect)) {
        state = 1;
        draw_tooltip = true;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state = 2;
            result = true;
        }
    }

    if (state == 0) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, LIGHTGRAY);
    } else if (state == 1) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, DARKGRAY);
    } else if (state == 2) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
    }

    imgui_label(rect, text, IMGUI_COLOR_BLACK);

    if (draw_tooltip) {
        imgui_internal_draw_tooltip();
    }

    return result;
}

IMGUI_API bool imgui_toggle_button(imgui_rect_t rect, const imgui_string_t text, bool toggle) {
    bool result = toggle;
    int state = 0;
    bool draw_tooltip = false;

    if (imgui_check_collision_rect_point(imgui_get_mouse_world_coordinate(), rect)) {
        state = 1;
        draw_tooltip = true;

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            state = 2;
        } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            result = !toggle;
        }
    }

    if (state == 0) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, LIGHTGRAY);
    } else if (state == 1) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, DARKGRAY);
    } else if (state == 2) {
        DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLUE);
    }

    imgui_label(rect, text, IMGUI_COLOR_BLACK);

    if (draw_tooltip) {
        imgui_internal_draw_tooltip();
    }
    return result;
}

IMGUI_API size_t imgui_tab_menu(imgui_rect_t rect, const imgui_string_t* tabs, size_t tab_count, size_t current_tab) {
    size_t result = current_tab;
    
    if (current_tab < 0 || current_tab >= tab_count) {
        return IMGUI_UNLIMITED;
    }

    imgui_rect_t tab_rect = { rect.x, rect.y, IMGUI_INTERNAL_DEFAULT_TAB_WIDTH, rect.height };

    for (size_t i = 0; i < tab_count; i++) {
        tab_rect.x = rect.x + (IMGUI_INTERNAL_DEFAULT_TAB_WIDTH + 4)*i;
        
        if (imgui_button(tab_rect, tabs[i])) {
            result = i;
        }
    }

    return result;
}

IMGUI_API size_t imgui_contex_menu(const imgui_string_t *items, size_t item_count) {
    size_t result = IMGUI_UNLIMITED;

    if (!imgui_internal_data.is_contex_menu_open) {
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
            imgui_internal_data.is_contex_menu_open = true;
            imgui_internal_data.contex_menu_pos = imgui_get_mouse_world_coordinate();

            size_t i = 0;
            for (; i < item_count && i < IMGUI_INTERNAL_DEFAULT_MAX_CONTEX_ITEMS; i++) {
                imgui_internal_data.contex_items[i] = items[i];
            }
            imgui_internal_data.contex_item_count = i;
        }
    }

    imgui_rect_t contex_rect = {
        .x = imgui_internal_data.contex_menu_pos.x,
        .y = imgui_internal_data.contex_menu_pos.y,
        .width = IMGUI_INTERNAL_DEFAULT_CONTEXT_MENU_WIDTH,
        .height = (imgui_internal_data.contex_item_count*IMGUI_INTERNAL_DEFAULT_CONTEX_ITEM_HEIGHT)};

    if (imgui_internal_data.is_contex_menu_open) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) {
            if (!imgui_check_collision_rect_point(imgui_get_mouse_world_coordinate(), contex_rect)) {
                imgui_internal_data.is_contex_menu_open = false;
                imgui_internal_data.contex_item_count = 0;
            }
        }

        for (size_t i = 0; i < imgui_internal_data.contex_item_count; i++) {
            imgui_rect_t item_rect = {
                .x = imgui_internal_data.contex_menu_pos.x,
                .y = imgui_internal_data.contex_menu_pos.y + (IMGUI_INTERNAL_DEFAULT_CONTEX_ITEM_HEIGHT*i),
                .width = IMGUI_INTERNAL_DEFAULT_CONTEXT_MENU_WIDTH,
                .height = IMGUI_INTERNAL_DEFAULT_CONTEX_ITEM_HEIGHT};

            if (imgui_button(item_rect, imgui_internal_data.contex_items[i])) {
                imgui_internal_data.is_contex_menu_open = false;
                result = i;
            }
        }
    }

    return result;
}

// NOTE: add horizontal padding
IMGUI_API float imgui_slider(imgui_rect_t rect, const imgui_string_t left_text, const imgui_string_t right_text, float value, float min, float max) {
    float result = 0.0f;

    value = Clamp(value, min, max);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(imgui_internal_vec2_to_Vector2(imgui_get_mouse_world_coordinate()), imgui_internal_rect_to_Rectangle(rect))) {
            if (!imgui_internal_data.is_slider_dragged) {
                imgui_internal_data.is_slider_dragged = true;
                imgui_internal_data.current_slider_rect = rect;
            }
        }
    } else {
        imgui_internal_data.is_slider_dragged = false;
    }

    float slider_width = 0.0f;
    float vertical_padding = 2;

    if (imgui_internal_data.is_slider_dragged == true && imgui_internal_rect_is_equal(rect, imgui_internal_data.current_slider_rect)) {
        slider_width = imgui_get_mouse_world_coordinate().x - rect.x;
        slider_width = Clamp(slider_width, 0.0f, rect.width);

    } else {
        slider_width = (value-min)/(max-min) * rect.width;
    }

    imgui_rect(rect, 0, IMGUI_COLOR_LIGHTGRAY, IMGUI_COLOR_BLANK);
    DrawRectangle(rect.x, rect.y + vertical_padding, slider_width, rect.height - (vertical_padding*2), SKYBLUE);

    result = (slider_width/rect.width)*max;

    return result;
}

IMGUI_API bool imgui_check_collision_rects(imgui_rect_t rect1, imgui_rect_t rect2) {
    return CheckCollisionRecs(imgui_internal_rect_to_Rectangle(rect1), imgui_internal_rect_to_Rectangle(rect2));
}

IMGUI_API bool imgui_check_collision_rect_point(imgui_vec2_t point, imgui_rect_t rect) {
    return CheckCollisionPointRec(imgui_internal_vec2_to_Vector2(point), imgui_internal_rect_to_Rectangle(rect));
}

#endif // IMGUI_IMPLEMENTATION

#endif // _IMGUI_H_