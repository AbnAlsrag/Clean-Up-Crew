#include "platform/platform.h"

#include <stdint.h>
#include <stdbool.h>
#include <raylib/raylib.h>
#include <raylib/rlgl.h>

typedef struct internal_data_t {
    platform_config_t config;
    bool is_viewport_active;
    viewport_t active_viewport;
} internal_data_t;

static internal_data_t internal_data = {0};

uint32_t get_raylib_config_flags(config_flags_t config_flags);
BlendMode get_raylib_blend_mode(blend_mode_t mode);
vec2f_t raylib_Vector2_to_vec2f(Vector2 vec);
Vector2 vec2f_to_raylib_Vector2(vec2f_t vec);
color_t raylib_Color_to_color(Color color);
Color color_to_raylib_Color(color_t color);
camera_t raylib_Camera2D_to_camera(Camera2D camera);
Camera2D camera_to_raylib_Camera2D(camera_t camera);
rectf_t raylib_Rectangle_to_rectf(Rectangle rect);
Rectangle rectf_to_raylib_Rectangle(rectf_t rect);
texture_t raylib_Texture2D_to_texture(Texture2D texture);
Texture2D texture_to_raylib_Texture2D(texture_t texture);

void platform_init(platform_config_t config) {
    SetConfigFlags(get_raylib_config_flags(config.config_flags));
    InitWindow(config.width, config.height, config.title);
    SetTargetFPS(config.target_fps);

    InitAudioDevice();

    internal_data.config = config;
    internal_data.config.width = GetScreenWidth();
    internal_data.config.height = GetScreenHeight();
}

void platform_reconfig(platform_config_t config) {
    SetConfigFlags(get_raylib_config_flags(config.config_flags));
    SetWindowTitle(internal_data.config.title);
    SetWindowSize(config.width, config.height);
    SetTargetFPS(config.target_fps);

    internal_data.config = config;
    internal_data.config.width = GetScreenWidth();
    internal_data.config.height = GetScreenHeight();
}

void platform_deinit(void) {
    CloseAudioDevice();
    CloseWindow();
}

bool platform_running(void) {
    return !WindowShouldClose();
}

bool platform_was_window_resized(void) {
    return IsWindowResized();
}

vec2f_t get_window_size(void) {
    return (vec2f_t) { GetScreenWidth(), GetScreenHeight() };
}

void clear_background(color_t color) {
    if (internal_data.is_viewport_active) {
        DrawRectangle(0, 0, internal_data.active_viewport.width, internal_data.active_viewport.height, color_to_raylib_Color(color));
    } else {
        ClearBackground(color_to_raylib_Color(color));
    }
}

void begin_drawing(void) {
    BeginDrawing();
}

void end_drawing(void) {
    EndDrawing();
}

void begin_viewport(viewport_t viewport) {
    rlViewport(viewport.x, GetScreenHeight()-viewport.height-viewport.y, viewport.width, viewport.height);
    rlMatrixMode(RL_PROJECTION);
    rlPushMatrix();
    rlLoadIdentity();
    rlOrtho(0.0, viewport.width, viewport.height, 0.0, -1.0, 1.0); // Consistent projection for fullscreen coordinates
    rlMatrixMode(RL_MODELVIEW);
    rlPushMatrix();
    rlLoadIdentity();

    internal_data.is_viewport_active = true;
    internal_data.active_viewport = viewport;
}

void end_viewport(void) {
    // Flush current batch for the viewport
    rlDrawRenderBatchActive();

    // Reset viewport to full screen
    rlViewport(0, 0, GetScreenWidth(), GetScreenHeight());

    // Reset projection matrix
    rlMatrixMode(RL_PROJECTION);
    rlPopMatrix();  // Restore previous matrix
    rlLoadIdentity();  // Ensure it's clean
    rlOrtho(0.0, GetScreenWidth(), GetScreenHeight(), 0.0, -1.0, 1.0);

    // Reset modelview matrix
    rlMatrixMode(RL_MODELVIEW);
    rlPopMatrix();  // Restore previous matrix
    rlLoadIdentity();

    internal_data.is_viewport_active = false;
}

void begin_camera(camera_t camera) {
    BeginMode2D(camera_to_raylib_Camera2D(camera));
}

void end_camera(void) {
    EndMode2D();
}

void begin_blend_mode(blend_mode_t mode) {
    BeginBlendMode(get_raylib_blend_mode(mode));
}

void end_blend_mode(void) {
    EndBlendMode();
}

vec2f_t get_world_to_screen(camera_t camera, vec2f_t pos) {
    return raylib_Vector2_to_vec2f(GetWorldToScreen2D(vec2f_to_raylib_Vector2(pos), camera_to_raylib_Camera2D(camera)));
}

vec2f_t get_screen_to_world(camera_t camera, vec2f_t pos) {
    return raylib_Vector2_to_vec2f(GetScreenToWorld2D(vec2f_to_raylib_Vector2(pos), camera_to_raylib_Camera2D(camera)));
}

texture_t load_texture(char *file_path) {
    return raylib_Texture2D_to_texture(LoadTexture(file_path));
}

// TODO: Solve the `IsTextureValid` issue
bool is_texture_valid(texture_t texture) {
    return IsTextureValid(texture_to_raylib_Texture2D(texture));
}

void unload_texture(texture_t texture) {
    UnloadTexture(texture_to_raylib_Texture2D(texture));
}

void draw_line(vec2f_t start, vec2f_t end, float thickness, color_t color) {
    DrawLineEx(vec2f_to_raylib_Vector2(start), vec2f_to_raylib_Vector2(end), thickness, color_to_raylib_Color(color));
}

void draw_triangle(vec2f_t point0, vec2f_t point1, vec2f_t point2, color_t color) {
    DrawTriangle(vec2f_to_raylib_Vector2(point0), vec2f_to_raylib_Vector2(point1), vec2f_to_raylib_Vector2(point2), color_to_raylib_Color(color));
}

void draw_rect(rectf_t rect, vec2f_t origin, float rotation, color_t color) {
    DrawRectanglePro(rectf_to_raylib_Rectangle(rect), vec2f_to_raylib_Vector2(origin), rotation, color_to_raylib_Color(color));
}

void draw_circle(vec2f_t center, float radius, color_t color) {
    DrawCircle(center.x, center.y, radius, color_to_raylib_Color(color));
}

void draw_circle_sector(vec2f_t center, float radius, float start_angle, float end_angle, color_t color) {
    DrawCircleSector(vec2f_to_raylib_Vector2(center), radius, start_angle, end_angle, 36, color_to_raylib_Color(color));
}

void draw_texture(texture_t texture, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint) {
    Texture2D raylib_texture = texture_to_raylib_Texture2D(texture);

    DrawTexturePro(raylib_texture, rectf_to_raylib_Rectangle(source), rectf_to_raylib_Rectangle(dest), vec2f_to_raylib_Vector2(origin), rotation, color_to_raylib_Color(tint));
}

float get_frame_time(void) {
    return GetFrameTime();
}

double get_time(void) {
    return GetTime();
}

uint32_t get_fps(void) {
    return GetFPS();
}

void open_url(char *url) {
    OpenURL(url);
}

uint32_t get_raylib_config_flags(config_flags_t config_flags) {
    uint32_t result = 0;

    if (config_flags & CONFIG_FLAG_VSYNC) {
        result |= FLAG_VSYNC_HINT;
    }

    if (config_flags & CONFIG_FLAG_FULLSCREEN) {
        result |= FLAG_FULLSCREEN_MODE | FLAG_WINDOW_MAXIMIZED;
    }

    if (config_flags & CONFIG_FLAG_BORDERLESS_WINDOWED) {
        result |= FLAG_BORDERLESS_WINDOWED_MODE;
    }

    return result;
}

BlendMode get_raylib_blend_mode(blend_mode_t mode) {
    return mode;
}

vec2f_t raylib_Vector2_to_vec2f(Vector2 vec) {
    return (vec2f_t) { vec.x, vec.y };
}

Vector2 vec2f_to_raylib_Vector2(vec2f_t vec) {
    return (Vector2) { vec.x, vec.y };
}

color_t raylib_Color_to_color(Color color) {
    return (color_t) { color.r, color.g, color.b, color.a };
}

Color color_to_raylib_Color(color_t color) {
    return (Color) { color.r, color.g, color.b, color.a };
}

camera_t raylib_Camera2D_to_camera(Camera2D camera) {
    return (camera_t) {
        .offset = raylib_Vector2_to_vec2f(camera.offset),
        .target = raylib_Vector2_to_vec2f(camera.target),
        .rotation = camera.rotation,
        .zoom = camera.zoom
    };
}

Camera2D camera_to_raylib_Camera2D(camera_t camera) {
    return (Camera2D) {
        .offset = vec2f_to_raylib_Vector2(camera.offset),
        .target = vec2f_to_raylib_Vector2(camera.target),
        .rotation = camera.rotation,
        .zoom = camera.zoom
    };
}
rectf_t raylib_Rectangle_to_rectf(Rectangle rect) {
    return (rectf_t) { rect.x, rect.y, rect.width, rect.height };
}

Rectangle rectf_to_raylib_Rectangle(rectf_t rect) {
    return (Rectangle) { rect.x, rect.y, rect.width, rect.height };
}

texture_t raylib_Texture2D_to_texture(Texture2D texture) {
    texture_t result = {
        .id = texture.id,
        .width = texture.width,
        .height = texture.height
    };

    return result;
}

// NOTE: The engine assumes that all textures are PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 this is fine because we will use our own format but it would be safer to assert when that isn't the case
Texture2D texture_to_raylib_Texture2D(texture_t texture) {
    Texture2D result = {
        .id = texture.id,
        .width = texture.width,
        .height = texture.height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    return result;
}