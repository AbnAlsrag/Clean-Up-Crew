#include "platform/platform.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#define DEFAULT_FONT_GLYPH_COUNT 224

typedef struct internal_data_t {
    platform_config_t config;
    bool is_viewport_active;
    platform_viewport_t active_viewport;
    bool is_camera_active;
    platform_camera_t active_camera;
    bool is_wireframe_mode_active;
    bool toggle_borderless_windowed;
    ssize_t text_line_spacing;
    platform_font_t default_font;
    platform_glyph_info_t default_font_glyph_info[DEFAULT_FONT_GLYPH_COUNT];
} internal_data_t;

// int x = sizeof(internal_data_t);

static internal_data_t internal_data = {0};

// TODO: make the internal functions static
uint32_t get_raylib_config_flags(platform_config_flags_t config_flags);
BlendMode get_raylib_blend_mode(platform_blend_mode_t mode);
KeyboardKey get_raylib_keyboard_key(platform_keycode_t keycode);
platform_keycode_t get_key_code(KeyboardKey key);
MouseButton get_raylib_mouse_button(platform_mouse_button_t mouse_button);
GamepadButton gamepad_button_to_raylib_GamepadButton(platform_gamepad_button_t gamepad_button);
GamepadAxis gamepad_axis_to_raylib_GamepadAxis(platform_gamepad_axis_t gamepad_axis);
vec2f_t raylib_Vector2_to_vec2f(Vector2 vec);
Vector2 vec2f_to_raylib_Vector2(vec2f_t vec);
color_t raylib_Color_to_color(Color color);
Color color_to_raylib_Color(color_t color);
platform_camera_t raylib_Camera2D_to_camera(Camera2D camera);
Camera2D camera_to_raylib_Camera2D(platform_camera_t camera);
rectf_t raylib_Rectangle_to_rectf(Rectangle rect);
Rectangle rectf_to_raylib_Rectangle(rectf_t rect);
platform_texture_t raylib_Texture2D_to_texture(Texture2D texture);
Texture2D texture_to_raylib_Texture2D(platform_texture_t texture);
platform_sound_t raylib_Sound_to_sound(Sound sound);
Sound sound_to_raylib_Sound(platform_sound_t sound);
RenderTexture2D frame_buffer_to_raylibe_RenderTexture2D(platform_frame_buffer_t frame_buffer);
size_t text_lenght(const char *text);
platform_codepoint_t get_next_codepoint(const char *text, size_t *out_codepoint_size);
size_t get_code_point_index(platform_font_t font, platform_codepoint_t codepoint, bool *out_found);

#include <stdio.h>

void platform_init(platform_config_t config) {
    InitWindow(config.width, config.height, config.title);
    SetWindowState(get_raylib_config_flags(config.config_flags));

    if (config.config_flags & PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED) {
        ToggleFullscreen();
        ToggleFullscreen();
        SetWindowState(get_raylib_config_flags(config.config_flags));
    }
    
    SetTargetFPS(config.target_fps);

    InitAudioDevice();

    internal_data.config = config;
    internal_data.config.width = GetScreenWidth();
    internal_data.config.height = GetScreenHeight();
    internal_data.text_line_spacing = 2;


    {
        Font default_font = GetFontDefault();

        internal_data.default_font.glyphs = internal_data.default_font_glyph_info;
        internal_data.default_font.glyph_padding = 0;
        internal_data.default_font.glyph_count = default_font.glyphCount;
        internal_data.default_font.size = default_font.baseSize;
        internal_data.default_font.texture = raylib_Texture2D_to_texture(default_font.texture);
        
        for (size_t i = 0; i < default_font.glyphCount && i < DEFAULT_FONT_GLYPH_COUNT; i++) {
            GlyphInfo ray_glyph = default_font.glyphs[i];
            Rectangle ray_glyph_rect = default_font.recs[i];
            
            internal_data.default_font.glyphs[i].codepoint = ray_glyph.value;
            internal_data.default_font.glyphs[i].offset_x = ray_glyph.offsetX;
            internal_data.default_font.glyphs[i].offset_y = ray_glyph.offsetY;
            internal_data.default_font.glyphs[i].advance_x = ray_glyph.advanceX;
            internal_data.default_font.glyphs[i].rect = raylib_Rectangle_to_rectf(ray_glyph_rect);
        }
    }
}

void platform_reconfig(platform_config_t config) {
    SetWindowState(get_raylib_config_flags(config.config_flags));
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

vec2f_t platform_get_window_size(void) {
    return (vec2f_t) { GetScreenWidth(), GetScreenHeight() };
}

void *platform_get_native_window_handle(void) {
    return GetWindowHandle();
}

bool platform_is_key_pressed(platform_keycode_t keycode) {
    return IsKeyPressed(get_raylib_keyboard_key(keycode));
}

bool platform_is_key_down(platform_keycode_t keycode) {
    return IsKeyDown(get_raylib_keyboard_key(keycode));
}

bool platform_is_key_repeat(platform_keycode_t keycode) {
    return IsKeyPressedRepeat(get_raylib_keyboard_key(keycode));
}

bool platform_is_key_released(platform_keycode_t keycode) {
    return IsKeyReleased(get_raylib_keyboard_key(keycode));
}

bool platform_is_key_up(platform_keycode_t keycode) {
    return IsKeyUp(get_raylib_keyboard_key(keycode));
}

platform_keycode_t platform_get_pressed_key(void) {
    return get_key_code(GetKeyPressed());
}

bool platform_is_mouse_button_pressed(platform_mouse_button_t mouse_button) {
    return IsMouseButtonPressed(get_raylib_mouse_button(mouse_button));
}

bool platform_is_mouse_button_down(platform_mouse_button_t mouse_button) {
    return IsMouseButtonDown(get_raylib_mouse_button(mouse_button));
}

bool platform_is_mouse_button_released(platform_mouse_button_t mouse_button) {
    return IsMouseButtonReleased(get_raylib_mouse_button(mouse_button));
}

bool platform_is_mouse_button_up(platform_mouse_button_t mouse_button) {
    return IsMouseButtonUp(get_raylib_mouse_button(mouse_button));
}

bool platform_is_mouse_on_screen(void) {
    return IsCursorOnScreen();
}

vec2f_t platform_get_mouse_position(void) {
    return raylib_Vector2_to_vec2f(GetMousePosition());
}

void platform_set_mouse_position(vec2f_t position) {
    SetMousePosition(position.x, position.y);
}

vec2f_t platform_get_mouse_delta(void) {
    return raylib_Vector2_to_vec2f(GetMouseDelta());
}

vec2f_t platform_get_mouse_wheel(void) {
    return raylib_Vector2_to_vec2f(GetMouseWheelMoveV());
}

bool platform_is_gamepad_present(gamepad_id_t id) {
    return IsGamepadAvailable(id);
}

bool platform_is_gamepad_button_pressed(gamepad_id_t id, platform_gamepad_button_t button) {
    return IsGamepadButtonPressed(id, gamepad_button_to_raylib_GamepadButton(button));
}

bool platform_is_gamepad_button_down(gamepad_id_t id, platform_gamepad_button_t button) {
    return IsGamepadButtonDown(id, gamepad_button_to_raylib_GamepadButton(button));
}

bool platform_is_gamepad_button_released(gamepad_id_t id, platform_gamepad_button_t button) {
    return IsGamepadButtonReleased(id, gamepad_button_to_raylib_GamepadButton(button));
}

bool platform_is_gamepad_button_up(gamepad_id_t id, platform_gamepad_button_t button) {
    return IsGamepadButtonUp(id, gamepad_button_to_raylib_GamepadButton(button));
}

float platform_get_gamepad_axis(gamepad_id_t id, platform_gamepad_axis_t axis) {
    return GetGamepadAxisMovement(id, gamepad_axis_to_raylib_GamepadAxis(axis));
}

void platform_set_master_volume(float volume) {
    SetMasterVolume(volume);
}

float platform_get_master_volume(void) {
    return GetMasterVolume();
}

platform_sound_t platform_load_sound(char *file_path) {
    return raylib_Sound_to_sound(LoadSound(file_path));
}

void platform_unload_sound(platform_sound_t sound) {
    UnloadSound(sound_to_raylib_Sound(sound));
}

void platform_play_sound(platform_sound_t sound) {
    PlaySound(sound_to_raylib_Sound(sound));
}

void platform_stop_sound(platform_sound_t sound) {
    StopSound(sound_to_raylib_Sound(sound));
}

void platform_pause_sound(platform_sound_t sound) {
    PauseSound(sound_to_raylib_Sound(sound));
}

void platform_resume_sound(platform_sound_t sound) {
    ResumeSound(sound_to_raylib_Sound(sound));
}

bool platform_is_sound_playing(platform_sound_t sound) {
    return IsSoundPlaying(sound_to_raylib_Sound(sound));
}

void platform_set_sound_volume(platform_sound_t sound, float volume) {
    SetSoundVolume(sound_to_raylib_Sound(sound), volume);
}

void platform_set_sound_pitch(platform_sound_t sound, float pitch) {
    SetSoundPitch(sound_to_raylib_Sound(sound), pitch);
}

void platform_set_sound_pan(platform_sound_t sound, float pan) {
    SetSoundPan(sound_to_raylib_Sound(sound), pan);
}

void platform_clear_background(color_t color) {
    bool disabled_camera = false;
    bool disabled_wireframe = false;

    if (internal_data.is_viewport_active) {
        if (internal_data.is_camera_active) {
            platform_end_camera();
            disabled_camera = true;
        }

        if (internal_data.is_wireframe_mode_active) {
            platform_end_wireframe_mode();
            disabled_wireframe = true;
        }

        DrawRectangle(0, 0, internal_data.active_viewport.width, internal_data.active_viewport.height, color_to_raylib_Color(color));

        if (disabled_camera) {
            platform_begin_camera(internal_data.active_camera);
        }

        if (disabled_wireframe) {
            platform_begin_wireframe_mode();
        }
    } else {
        ClearBackground(color_to_raylib_Color(color));
    }
}

void platform_begin_drawing(void) {
    BeginDrawing();
}

void platform_end_drawing(void) {
    EndDrawing();
}

void platform_begin_viewport(platform_viewport_t viewport) {
    rlDrawRenderBatchActive();
    
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

void platform_end_viewport(void) {
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

void platform_begin_frame_buffer(platform_frame_buffer_t frame_buffer) {
    rlDrawRenderBatchActive();

    rlDisableBackfaceCulling();

    rlEnableFramebuffer(frame_buffer.id);

    rlViewport(0, 0, frame_buffer.texture.width, frame_buffer.texture.height);

    // Set up projection matrix
    rlMatrixMode(RL_PROJECTION);
    rlPushMatrix();
    rlLoadIdentity();
    rlOrtho(0.0, frame_buffer.texture.width, frame_buffer.texture.height, 0.0, -1.0, 1.0);
    // rlOrtho(0.0, frame_buffer.texture.width, 0.0, frame_buffer.texture.height, -1.0, 1.0);

    // Set up modelview matrix with flip
    rlMatrixMode(RL_MODELVIEW);
    rlPushMatrix();
    rlLoadIdentity();
    rlTranslatef(0.0f, frame_buffer.texture.height, 0.0f);
    rlScalef(1.0f, -1.0f, 1.0f);
}

void platform_end_frame_buffer(void) {
    rlDrawRenderBatchActive();

    rlEnableBackfaceCulling();

    rlDisableFramebuffer();

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
}

void platform_begin_camera(platform_camera_t camera) {
    BeginMode2D(camera_to_raylib_Camera2D(camera));

    internal_data.is_camera_active = true;
    internal_data.active_camera = camera;
}

void platform_end_camera(void) {
    EndMode2D();

    internal_data.is_camera_active = false;
}

void platform_begin_blend_mode(platform_blend_mode_t mode) {
    BeginBlendMode(get_raylib_blend_mode(mode));
}

void platform_end_blend_mode(void) {
    EndBlendMode();
}

void platform_begin_scissor_mode(rectf_t rect) {
    BeginScissorMode(rect.x, rect.y, rect.width, rect.height);
}

void platform_end_scissor_mode(void) {
    EndScissorMode();
}

void platform_begin_wireframe_mode(void) {
    rlDrawRenderBatchActive();

    rlEnableWireMode();

    internal_data.is_wireframe_mode_active = true;
}

void platform_end_wireframe_mode(void) {
    rlDrawRenderBatchActive();
    
    rlDisableWireMode();

    internal_data.is_wireframe_mode_active = false;
}

vec2f_t platform_get_world_to_screen(platform_camera_t camera, vec2f_t pos) {
    return raylib_Vector2_to_vec2f(GetWorldToScreen2D(vec2f_to_raylib_Vector2(pos), camera_to_raylib_Camera2D(camera)));
}

vec2f_t platform_get_screen_to_world(platform_camera_t camera, vec2f_t pos) {
    return raylib_Vector2_to_vec2f(GetScreenToWorld2D(vec2f_to_raylib_Vector2(pos), camera_to_raylib_Camera2D(camera)));
}

platform_texture_t platform_load_texture(const char *file_path) {
    return raylib_Texture2D_to_texture(LoadTexture(file_path));
}

// TODO: Solve the `IsTextureValid` issue
bool platform_is_texture_valid(platform_texture_t texture) {
    return IsTextureValid(texture_to_raylib_Texture2D(texture));
}

rectf_t platform_get_texture_src_rect(platform_texture_t texture) {
    return (rectf_t) { 0, 0, texture.width, texture.height };
}

void platform_unload_texture(platform_texture_t texture) {
    UnloadTexture(texture_to_raylib_Texture2D(texture));
}

// TODO: validate the framebuffer and check for errors
platform_frame_buffer_t platform_load_frame_buffer(uint32_t width, uint32_t height) {
    platform_frame_buffer_t result = {0};

    result.id = rlLoadFramebuffer();

    if (result.id > 0) {
        rlEnableFramebuffer(result.id);

        result.texture.id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        result.texture.width = width;
        result.texture.height = height;

        rlFramebufferAttach(result.id, result.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);

        rlDisableFramebuffer();
    }

    return result;
}

void platform_unload_frame_buffer(platform_frame_buffer_t frame_buffer) {
    if (frame_buffer.id > 0) {
        if (frame_buffer.texture.id > 0) {
            rlUnloadTexture(frame_buffer.texture.id);
        }
        
        rlUnloadFramebuffer(frame_buffer.id);
    }
}

void platform_set_text_line_spacing(ssize_t spacing) {
    internal_data.text_line_spacing = spacing;
}

platform_font_t platform_get_default_font(void) {
    return internal_data.default_font;
}

// TODO: add logging
platform_font_t platform_load_font_from_ttf_file(const char *file_path, uint32_t font_size, platform_codepoint_t *codepoints, size_t codepoint_count) {
    platform_font_t result = {0};

    if (codepoint_count == 0) {
        return result;
    }

    if (codepoints == NULL) {
        return result;
    }

    Font font = LoadFontEx(file_path, font_size, (int*)((void*)codepoints), codepoint_count);

    if (codepoint_count != font.glyphCount) {
        return result;
    }

    platform_glyph_info_t *glyph_info = RL_MALLOC(sizeof(platform_glyph_info_t)*codepoint_count);

    if (glyph_info == NULL) {
        return result;
    }

    for (size_t i = 0; i < codepoint_count; i++) {
        GlyphInfo ray_glyph = font.glyphs[i];
        Rectangle ray_glyph_rect = font.recs[i];
        
        glyph_info[i].codepoint = ray_glyph.value;
        glyph_info[i].offset_x = ray_glyph.offsetX;
        glyph_info[i].offset_y = ray_glyph.offsetY;
        glyph_info[i].advance_x = ray_glyph.advanceX;
        glyph_info[i].rect = raylib_Rectangle_to_rectf(ray_glyph_rect);

        if (ray_glyph.value == 0x0631) {
            glyph_info[i].offset_x = 0;
        }

        if (ray_glyph.value == 0x0648) {
            glyph_info[i].offset_x = 0;
        }
    }

    result.size = font_size;
    result.glyph_padding = font.glyphPadding;
    result.glyph_count = codepoint_count;
    result.glyphs = glyph_info;
    result.texture = raylib_Texture2D_to_texture(font.texture);

    UnloadFontData(font.glyphs, font.glyphCount);
    RL_FREE(font.recs);

    return result;
}

bool platform_is_font_valid(platform_font_t font) {
    return ((font.size > 0) &&
            (font.glyph_count > 0) &&
            (font.glyphs != NULL) &&
            (platform_is_texture_valid(font.texture)));
}

void platform_unload_font(platform_font_t font) {
    if (!platform_is_font_valid(font)) {
        return;
    }
    
    if (platform_is_texture_valid(font.texture)) {
        platform_unload_texture(font.texture);
    }

    if (font.glyphs != NULL) {
        RL_FREE(font.glyphs);
    }
}

// TODO: test this functions
vec2f_t platform_measure_text(platform_font_t font, const char *text, float font_size, float spacing) {
    vec2f_t text_size = VECTOR2_ZERO;

    if (text == NULL) {
        return VECTOR2_ZERO;
    }

    if (text[0] == '\0') {
        return VECTOR2_ZERO;
    }

    if (!platform_is_font_valid(font)) {
        return VECTOR2_ZERO;
    }

    size_t text_len = text_lenght(text);

    size_t char_counter = 0;
    size_t tmp_char_counter = 0;

    float text_width = 0.0f;
    float current_line_width = 0.0f;

    float text_height = font_size;

    float scale_factor = font_size/font.size;

    for (size_t i = 0; i < text_len;) {
        size_t codepoint_byte_size = 0;
        platform_codepoint_t codepoint = get_next_codepoint(&text[i], &codepoint_byte_size);
        size_t codepoint_index = get_code_point_index(font, codepoint, NULL);

        tmp_char_counter += 1;

        if (codepoint != '\n') {
            if (font.glyphs[codepoint_index].advance_x != 0) {
                current_line_width += font.glyphs[codepoint_index].advance_x;
            } else {
                current_line_width += (font.glyphs[codepoint_index].rect.width + font.glyphs[codepoint_index].offset_x);
            }
        } else {
            if (text_width < current_line_width) {
                text_width = current_line_width;
            }

            tmp_char_counter = 0;
            current_line_width = 0;

            text_height += (font_size + internal_data.text_line_spacing);
        }

        if (char_counter < tmp_char_counter) {
            char_counter = tmp_char_counter;
        }

        if (codepoint_byte_size == 0) {
            break;
        }

        i += codepoint_byte_size;
    }

    if (text_width < current_line_width) {
        text_width = current_line_width;
    }

    text_size.x = text_width*scale_factor + (float)((char_counter - 1)*spacing);
    text_size.y = text_height;

    return text_size;
}

void platform_draw_fps(vec2f_t pos) {
    DrawFPS(pos.x, pos.y);
}

void platform_draw_pixel(vec2f_t pos, color_t color) {
    DrawPixelV(vec2f_to_raylib_Vector2(pos), color_to_raylib_Color(color));
}

void platform_draw_line(vec2f_t start, vec2f_t end, float thickness, color_t tint) {
    DrawLineEx(vec2f_to_raylib_Vector2(start), vec2f_to_raylib_Vector2(end), thickness, color_to_raylib_Color(tint));
}

void platform_draw_triangle(triangle_t triangle, color_t tint) {
    DrawTriangle(vec2f_to_raylib_Vector2(triangle.point0), vec2f_to_raylib_Vector2(triangle.point1), vec2f_to_raylib_Vector2(triangle.point2), color_to_raylib_Color(tint));
}

// TODO: consider early returning if alpha is 0
void platform_draw_triangles(const triangle_t *triangles, size_t triangle_count, color_t tint) {
    if (triangles == NULL) {
        return;
    }

    if (triangle_count == 0) {
        return;
    }

    for (size_t i = 0; i < triangle_count; i++) {
        platform_draw_triangle(triangles[i], tint);
    }
}

// TODO: consider early returning if alpha is 0
void platform_draw_triangle_strip(const vec2f_t *points, size_t point_count, color_t tint) {
    if (points == NULL) {
        return;
    }

    if (point_count == 0) {
        return;
    }
    
    DrawTriangleStrip(((Vector2*)(void*)points), point_count, color_to_raylib_Color(tint));
}

// TODO: consider early returning if alpha is 0
void platform_draw_triangle_fan(const vec2f_t *points, size_t point_count, color_t tint) {
    if (points == NULL) {
        return;
    }

    if (point_count == 0) {
        return;
    }
    
    DrawTriangleFan(((Vector2*)(void*)points), point_count, color_to_raylib_Color(tint));
}

void platform_draw_rect(rectf_t rect, vec2f_t origin, float rotation, color_t tint) {
    DrawRectanglePro(rectf_to_raylib_Rectangle(rect), vec2f_to_raylib_Vector2(origin), rotation, color_to_raylib_Color(tint));
}

void platform_draw_circle(vec2f_t center, float radius, color_t tint) {
    DrawCircle(center.x, center.y, radius, color_to_raylib_Color(tint));
}

void platform_draw_circle_sector(vec2f_t center, float radius, float start_angle, float end_angle, color_t tint) {
    // DrawCircleSector(vec2f_to_raylib_Vector2(center), radius, start_angle, end_angle, 36, color_to_raylib_Color(color));
    DrawCircleSector(vec2f_to_raylib_Vector2(center), radius, start_angle, end_angle, 0, color_to_raylib_Color(tint));
}

void platform_draw_ring(vec2f_t center, float outer_radius, float inner_radius, float start_angle, float end_angle, color_t tint) {
    DrawRing(vec2f_to_raylib_Vector2(center), inner_radius, outer_radius, start_angle, end_angle, 36, color_to_raylib_Color(tint));
}

void platform_draw_texture(platform_texture_t texture, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint) {
    Texture2D raylib_texture = texture_to_raylib_Texture2D(texture);

    DrawTexturePro(raylib_texture, rectf_to_raylib_Rectangle(source), rectf_to_raylib_Rectangle(dest), vec2f_to_raylib_Vector2(origin), rotation, color_to_raylib_Color(tint));
}

// NOTE: see performance diff between rlBlitFramebuffer and DrawTexturePro
void platform_draw_frame_buffer(platform_frame_buffer_t frame_buffer, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint) {
    Texture2D raylib_texture = texture_to_raylib_Texture2D(frame_buffer.texture);

    DrawTexturePro(raylib_texture, rectf_to_raylib_Rectangle(source), rectf_to_raylib_Rectangle(dest), vec2f_to_raylib_Vector2(origin), rotation, color_to_raylib_Color(tint));
}

void platform_draw_text(platform_font_t font, const char *text, vec2f_t pos, vec2f_t origin, float rotation, float font_size, float spacing, color_t tint) {
    rlPushMatrix();

        rlRotatef(rotation, 0.0f, 0.0f, 1.0f);
        rlTranslatef(-origin.x, -origin.y, 0.0f);

        // NOTE: idk
        if (!platform_is_texture_valid(font.texture)) {
            return;
        }

        size_t text_len = text_lenght(text);

        float offset_x = 0.0f;
        float offset_y = 0.0f;

        float scale_factor = font_size/font.size;

        for (size_t i = 0; i < text_len;) {
            // TODO: handle when the codepoint isn't found and can't fallback
            size_t codepoint_byte_size = 0;
            platform_codepoint_t codepoint = get_next_codepoint(&text[i], &codepoint_byte_size);
            size_t index = get_code_point_index(font, codepoint, NULL);

            if (codepoint == '\n') {
                offset_y += (font_size + internal_data.text_line_spacing);
                offset_x = 0.0f;
            } else {
                if ((codepoint != ' ') && (codepoint != '\t')) {
                    platform_draw_codepoint(font, codepoint, (vec2f_t) { pos.x + offset_x, pos.y + offset_y }, font_size, tint);
                }

                if (font.glyphs[index].advance_x == 0) {
                    offset_x += ((float)font.glyphs[index].rect.width*scale_factor + spacing);
                } else {
                    offset_x += ((float)font.glyphs[index].advance_x + spacing);
                }
            }

            i += codepoint_byte_size;
        }

    rlPopMatrix();
}

void platform_draw_codepoint(platform_font_t font, platform_codepoint_t codepoint, vec2f_t position, float font_size, color_t tint) {
    bool found = false;
    size_t index = get_code_point_index(font, codepoint, &found);
    // printf("%u index = %zu, found = %d\n", codepoint, index, found);
    float scale_factor = font_size/font.size;
    // printf("glyph_padding = %u, scale_factor = %f\n", font.glyph_padding, scale_factor);

    rectf_t dest = { position.x + font.glyphs[index].offset_x*scale_factor - (float)font.glyph_padding*scale_factor,
                      position.y + font.glyphs[index].offset_y*scale_factor - (float)font.glyph_padding*scale_factor,
                      (font.glyphs[index].rect.width + 2.0f*font.glyph_padding)*scale_factor,
                      (font.glyphs[index].rect.height + 2.0f*font.glyph_padding)*scale_factor };

    rectf_t src = { font.glyphs[index].rect.x - (float)font.glyph_padding, font.glyphs[index].rect.y - (float)font.glyph_padding,
                    font.glyphs[index].rect.width + 2.0f*font.glyph_padding, font.glyphs[index].rect.height + 2.0f*font.glyph_padding };

    platform_draw_texture(font.texture, src, dest, VECTOR2_ZERO, 0.0f, tint);
}

float platform_get_delta_time(void) {
    return GetFrameTime();
}

double platform_get_time(void) {
    return GetTime();
}

uint32_t platform_get_fps(void) {
    return GetFPS();
}

void platform_wait(double secs) {
    WaitTime(secs);
}

void platform_open_url(char *url) {
    OpenURL(url);
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// INTERNAL /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

uint32_t get_raylib_config_flags(platform_config_flags_t config_flags) {
    uint32_t result = 0;

    if (config_flags & PLATFORM_CONFIG_FLAG_VSYNC) {
        result |= FLAG_VSYNC_HINT;
    }

    if (config_flags & PLATFORM_CONFIG_FLAG_FULLSCREEN) {
        result |= FLAG_FULLSCREEN_MODE;
    }

    if (config_flags & PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED) {
        // internal_data.toggle_borderless_windowed = true;
        // result |= FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_MAXIMIZED;
        result |= FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_MAXIMIZED;
    }

    if (config_flags & PLATFORM_CONFIG_FLAG_RESIZABLE) {
        result |= FLAG_WINDOW_RESIZABLE;
    }

    return result;
}

BlendMode get_raylib_blend_mode(platform_blend_mode_t mode) {
    return mode;
}

KeyboardKey get_raylib_keyboard_key(platform_keycode_t keycode) {
    switch (keycode) {
    case PLATFORM_KEYCODE_NONE: return KEY_NULL;
    case PLATFORM_KEYCODE_APOSTROPHE: return KEY_APOSTROPHE;
    case PLATFORM_KEYCODE_COMMA: return KEY_COMMA;
    case PLATFORM_KEYCODE_MINUS: return KEY_MINUS;
    case PLATFORM_KEYCODE_PERIOD: return KEY_PERIOD;
    case PLATFORM_KEYCODE_SLASH: return KEY_SLASH;
    case PLATFORM_KEYCODE_ZERO: return KEY_ZERO;
    case PLATFORM_KEYCODE_ONE: return KEY_ONE;
    case PLATFORM_KEYCODE_TWO: return KEY_TWO;
    case PLATFORM_KEYCODE_THREE: return KEY_THREE;
    case PLATFORM_KEYCODE_FOUR: return KEY_FOUR;
    case PLATFORM_KEYCODE_FIVE: return KEY_FIVE;
    case PLATFORM_KEYCODE_SIX: return KEY_SIX;
    case PLATFORM_KEYCODE_SEVEN: return KEY_SEVEN;
    case PLATFORM_KEYCODE_EIGHT: return KEY_EIGHT;
    case PLATFORM_KEYCODE_NINE: return KEY_NINE;
    case PLATFORM_KEYCODE_SEMICOLON: return KEY_SEMICOLON;
    case PLATFORM_KEYCODE_EQUAL: return KEY_EQUAL;
    case PLATFORM_KEYCODE_A: return KEY_A;
    case PLATFORM_KEYCODE_B: return KEY_B;
    case PLATFORM_KEYCODE_C: return KEY_C;
    case PLATFORM_KEYCODE_D: return KEY_D;
    case PLATFORM_KEYCODE_E: return KEY_E;
    case PLATFORM_KEYCODE_F: return KEY_F;
    case PLATFORM_KEYCODE_G: return KEY_G;
    case PLATFORM_KEYCODE_H: return KEY_H;
    case PLATFORM_KEYCODE_I: return KEY_I;
    case PLATFORM_KEYCODE_J: return KEY_J;
    case PLATFORM_KEYCODE_K: return KEY_K;
    case PLATFORM_KEYCODE_L: return KEY_L;
    case PLATFORM_KEYCODE_M: return KEY_M;
    case PLATFORM_KEYCODE_N: return KEY_N;
    case PLATFORM_KEYCODE_O: return KEY_O;
    case PLATFORM_KEYCODE_P: return KEY_P;
    case PLATFORM_KEYCODE_Q: return KEY_Q;
    case PLATFORM_KEYCODE_R: return KEY_R;
    case PLATFORM_KEYCODE_S: return KEY_S;
    case PLATFORM_KEYCODE_T: return KEY_T;
    case PLATFORM_KEYCODE_U: return KEY_U;
    case PLATFORM_KEYCODE_V: return KEY_V;
    case PLATFORM_KEYCODE_W: return KEY_W;
    case PLATFORM_KEYCODE_X: return KEY_X;
    case PLATFORM_KEYCODE_Y: return KEY_Y;
    case PLATFORM_KEYCODE_Z: return KEY_Z;
    case PLATFORM_KEYCODE_LEFT_BRACKET: return KEY_LEFT_BRACKET;
    case PLATFORM_KEYCODE_BACKSLASH: return KEY_BACKSLASH;
    case PLATFORM_KEYCODE_RIGHT_BRACKET: return KEY_RIGHT_BRACKET;
    case PLATFORM_KEYCODE_GRAVE: return KEY_GRAVE;
    case PLATFORM_KEYCODE_SPACE: return KEY_SPACE;
    case PLATFORM_KEYCODE_ESCAPE: return KEY_ESCAPE;
    case PLATFORM_KEYCODE_ENTER: return KEY_ENTER;
    case PLATFORM_KEYCODE_TAB: return KEY_TAB;
    case PLATFORM_KEYCODE_BACKSPACE: return KEY_BACKSPACE;
    case PLATFORM_KEYCODE_INSERT: return KEY_INSERT;
    case PLATFORM_KEYCODE_DELETE: return KEY_DELETE;
    case PLATFORM_KEYCODE_RIGHT: return KEY_RIGHT;
    case PLATFORM_KEYCODE_LEFT: return KEY_LEFT;
    case PLATFORM_KEYCODE_DOWN: return KEY_DOWN;
    case PLATFORM_KEYCODE_UP: return KEY_UP;
    case PLATFORM_KEYCODE_PAGE_UP: return KEY_PAGE_UP;
    case PLATFORM_KEYCODE_PAGE_DOWN: return KEY_PAGE_DOWN;
    case PLATFORM_KEYCODE_HOME: return KEY_HOME;
    case PLATFORM_KEYCODE_END: return KEY_END;
    case PLATFORM_KEYCODE_CAPS_LOCK: return KEY_CAPS_LOCK;
    case PLATFORM_KEYCODE_SCROLL_LOCK: return KEY_SCROLL_LOCK;
    case PLATFORM_KEYCODE_NUM_LOCK: return KEY_NUM_LOCK;
    case PLATFORM_KEYCODE_PRINT_SCREEN: return KEY_PRINT_SCREEN;
    case PLATFORM_KEYCODE_PAUSE: return KEY_PAUSE;
    case PLATFORM_KEYCODE_F1: return KEY_F1;
    case PLATFORM_KEYCODE_F2: return KEY_F2;
    case PLATFORM_KEYCODE_F3: return KEY_F3;
    case PLATFORM_KEYCODE_F4: return KEY_F4;
    case PLATFORM_KEYCODE_F5: return KEY_F5;
    case PLATFORM_KEYCODE_F6: return KEY_F6;
    case PLATFORM_KEYCODE_F7: return KEY_F7;
    case PLATFORM_KEYCODE_F8: return KEY_F8;
    case PLATFORM_KEYCODE_F9: return KEY_F9;
    case PLATFORM_KEYCODE_F10: return KEY_F10;
    case PLATFORM_KEYCODE_F11: return KEY_F11;
    case PLATFORM_KEYCODE_F12: return KEY_F12;
    case PLATFORM_KEYCODE_LEFT_SHIFT: return KEY_LEFT_SHIFT;
    case PLATFORM_KEYCODE_LEFT_CONTROL: return KEY_LEFT_CONTROL;
    case PLATFORM_KEYCODE_LEFT_ALT: return KEY_LEFT_ALT;
    case PLATFORM_KEYCODE_LEFT_SUPER: return KEY_LEFT_SUPER;
    case PLATFORM_KEYCODE_RIGHT_SHIFT: return KEY_RIGHT_SHIFT;
    case PLATFORM_KEYCODE_RIGHT_CONTROL: return KEY_RIGHT_CONTROL;
    case PLATFORM_KEYCODE_RIGHT_ALT: return KEY_RIGHT_ALT;
    case PLATFORM_KEYCODE_RIGHT_SUPER: return KEY_RIGHT_SUPER;
    case PLATFORM_KEYCODE_KB_MENU: return KEY_KB_MENU;
    case PLATFORM_KEYCODE_KP_0: return KEY_KP_0;
    case PLATFORM_KEYCODE_KP_1: return KEY_KP_1;
    case PLATFORM_KEYCODE_KP_2: return KEY_KP_2;
    case PLATFORM_KEYCODE_KP_3: return KEY_KP_3;
    case PLATFORM_KEYCODE_KP_4: return KEY_KP_4;
    case PLATFORM_KEYCODE_KP_5: return KEY_KP_5;
    case PLATFORM_KEYCODE_KP_6: return KEY_KP_6;
    case PLATFORM_KEYCODE_KP_7: return KEY_KP_7;
    case PLATFORM_KEYCODE_KP_8: return KEY_KP_8;
    case PLATFORM_KEYCODE_KP_9: return KEY_KP_9;
    case PLATFORM_KEYCODE_KP_DECIMAL: return KEY_KP_DECIMAL;
    case PLATFORM_KEYCODE_KP_DIVIDE: return KEY_KP_DIVIDE;
    case PLATFORM_KEYCODE_KP_MULTIPLY: return KEY_KP_MULTIPLY;
    case PLATFORM_KEYCODE_KP_SUBTRACT: return KEY_KP_SUBTRACT;
    case PLATFORM_KEYCODE_KP_ADD: return KEY_KP_ADD;
    case PLATFORM_KEYCODE_KP_ENTER: return KEY_KP_ENTER;
    case PLATFORM_KEYCODE_KP_EQUAL: return KEY_KP_EQUAL;
    case PLATFORM_KEYCODE_BACK: return KEY_BACK;
    case PLATFORM_KEYCODE_MENU: return KEY_MENU;
    case PLATFORM_KEYCODE_VOLUME_UP: return KEY_VOLUME_UP;
    case PLATFORM_KEYCODE_VOLUME_DOWN: return KEY_VOLUME_DOWN;
    default: return KEY_NULL;
    }

    return KEY_NULL;
}

platform_keycode_t get_key_code(KeyboardKey key) {
    switch (key) {
    case KEY_NULL: return PLATFORM_KEYCODE_NONE;
    case KEY_APOSTROPHE: return PLATFORM_KEYCODE_APOSTROPHE;
    case KEY_COMMA: return PLATFORM_KEYCODE_COMMA;
    case KEY_MINUS: return PLATFORM_KEYCODE_MINUS;
    case KEY_PERIOD: return PLATFORM_KEYCODE_PERIOD;
    case KEY_SLASH: return PLATFORM_KEYCODE_SLASH;
    case KEY_ZERO: return PLATFORM_KEYCODE_ZERO;
    case KEY_ONE: return PLATFORM_KEYCODE_ONE;
    case KEY_TWO: return PLATFORM_KEYCODE_TWO;
    case KEY_THREE: return PLATFORM_KEYCODE_THREE;
    case KEY_FOUR: return PLATFORM_KEYCODE_FOUR;
    case KEY_FIVE: return PLATFORM_KEYCODE_FIVE;
    case KEY_SIX: return PLATFORM_KEYCODE_SIX;
    case KEY_SEVEN: return PLATFORM_KEYCODE_SEVEN;
    case KEY_EIGHT: return PLATFORM_KEYCODE_EIGHT;
    case KEY_NINE: return PLATFORM_KEYCODE_NINE;
    case KEY_SEMICOLON: return PLATFORM_KEYCODE_SEMICOLON;
    case KEY_EQUAL: return PLATFORM_KEYCODE_EQUAL;
    case KEY_A: return PLATFORM_KEYCODE_A;
    case KEY_B: return PLATFORM_KEYCODE_B;
    case KEY_C: return PLATFORM_KEYCODE_C;
    case KEY_D: return PLATFORM_KEYCODE_D;
    case KEY_E: return PLATFORM_KEYCODE_E;
    case KEY_F: return PLATFORM_KEYCODE_F;
    case KEY_G: return PLATFORM_KEYCODE_G;
    case KEY_H: return PLATFORM_KEYCODE_H;
    case KEY_I: return PLATFORM_KEYCODE_I;
    case KEY_J: return PLATFORM_KEYCODE_J;
    case KEY_K: return PLATFORM_KEYCODE_K;
    case KEY_L: return PLATFORM_KEYCODE_L;
    case KEY_M: return PLATFORM_KEYCODE_M;
    case KEY_N: return PLATFORM_KEYCODE_N;
    case KEY_O: return PLATFORM_KEYCODE_O;
    case KEY_P: return PLATFORM_KEYCODE_P;
    case KEY_Q: return PLATFORM_KEYCODE_Q;
    case KEY_R: return PLATFORM_KEYCODE_R;
    case KEY_S: return PLATFORM_KEYCODE_S;
    case KEY_T: return PLATFORM_KEYCODE_T;
    case KEY_U: return PLATFORM_KEYCODE_U;
    case KEY_V: return PLATFORM_KEYCODE_V;
    case KEY_W: return PLATFORM_KEYCODE_W;
    case KEY_X: return PLATFORM_KEYCODE_X;
    case KEY_Y: return PLATFORM_KEYCODE_Y;
    case KEY_Z: return PLATFORM_KEYCODE_Z;
    case KEY_LEFT_BRACKET: return PLATFORM_KEYCODE_LEFT_BRACKET;
    case KEY_BACKSLASH: return PLATFORM_KEYCODE_BACKSLASH;
    case KEY_RIGHT_BRACKET: return PLATFORM_KEYCODE_RIGHT_BRACKET;
    case KEY_GRAVE: return PLATFORM_KEYCODE_GRAVE;
    case KEY_SPACE: return PLATFORM_KEYCODE_SPACE;
    case KEY_ESCAPE: return PLATFORM_KEYCODE_ESCAPE;
    case KEY_ENTER: return PLATFORM_KEYCODE_ENTER;
    case KEY_TAB: return PLATFORM_KEYCODE_TAB;
    case KEY_BACKSPACE: return PLATFORM_KEYCODE_BACKSPACE;
    case KEY_INSERT: return PLATFORM_KEYCODE_INSERT;
    case KEY_DELETE: return PLATFORM_KEYCODE_DELETE;
    case KEY_RIGHT: return PLATFORM_KEYCODE_RIGHT;
    case KEY_LEFT: return PLATFORM_KEYCODE_LEFT;
    case KEY_DOWN: return PLATFORM_KEYCODE_DOWN;
    case KEY_UP: return PLATFORM_KEYCODE_UP;
    case KEY_PAGE_UP: return PLATFORM_KEYCODE_PAGE_UP;
    case KEY_PAGE_DOWN: return PLATFORM_KEYCODE_PAGE_DOWN;
    case KEY_HOME: return PLATFORM_KEYCODE_HOME;
    case KEY_END: return PLATFORM_KEYCODE_END;
    case KEY_CAPS_LOCK: return PLATFORM_KEYCODE_CAPS_LOCK;
    case KEY_SCROLL_LOCK: return PLATFORM_KEYCODE_SCROLL_LOCK;
    case KEY_NUM_LOCK: return PLATFORM_KEYCODE_NUM_LOCK;
    case KEY_PRINT_SCREEN: return PLATFORM_KEYCODE_PRINT_SCREEN;
    case KEY_PAUSE: return PLATFORM_KEYCODE_PAUSE;
    case KEY_F1: return PLATFORM_KEYCODE_F1;
    case KEY_F2: return PLATFORM_KEYCODE_F2;
    case KEY_F3: return PLATFORM_KEYCODE_F3;
    case KEY_F4: return PLATFORM_KEYCODE_F4;
    case KEY_F5: return PLATFORM_KEYCODE_F5;
    case KEY_F6: return PLATFORM_KEYCODE_F6;
    case KEY_F7: return PLATFORM_KEYCODE_F7;
    case KEY_F8: return PLATFORM_KEYCODE_F8;
    case KEY_F9: return PLATFORM_KEYCODE_F9;
    case KEY_F10: return PLATFORM_KEYCODE_F10;
    case KEY_F11: return PLATFORM_KEYCODE_F11;
    case KEY_F12: return PLATFORM_KEYCODE_F12;
    case KEY_LEFT_SHIFT: return PLATFORM_KEYCODE_LEFT_SHIFT;
    case KEY_LEFT_CONTROL: return PLATFORM_KEYCODE_LEFT_CONTROL;
    case KEY_LEFT_ALT: return PLATFORM_KEYCODE_LEFT_ALT;
    case KEY_LEFT_SUPER: return PLATFORM_KEYCODE_LEFT_SUPER;
    case KEY_RIGHT_SHIFT: return PLATFORM_KEYCODE_RIGHT_SHIFT;
    case KEY_RIGHT_CONTROL: return PLATFORM_KEYCODE_RIGHT_CONTROL;
    case KEY_RIGHT_ALT: return PLATFORM_KEYCODE_RIGHT_ALT;
    case KEY_RIGHT_SUPER: return PLATFORM_KEYCODE_RIGHT_SUPER;
    case KEY_KB_MENU: return PLATFORM_KEYCODE_KB_MENU;
    case KEY_KP_0: return PLATFORM_KEYCODE_KP_0;
    case KEY_KP_1: return PLATFORM_KEYCODE_KP_1;
    case KEY_KP_2: return PLATFORM_KEYCODE_KP_2;
    case KEY_KP_3: return PLATFORM_KEYCODE_KP_3;
    case KEY_KP_4: return PLATFORM_KEYCODE_KP_4;
    case KEY_KP_5: return PLATFORM_KEYCODE_KP_5;
    case KEY_KP_6: return PLATFORM_KEYCODE_KP_6;
    case KEY_KP_7: return PLATFORM_KEYCODE_KP_7;
    case KEY_KP_8: return PLATFORM_KEYCODE_KP_8;
    case KEY_KP_9: return PLATFORM_KEYCODE_KP_9;
    case KEY_KP_DECIMAL: return PLATFORM_KEYCODE_KP_DECIMAL;
    case KEY_KP_DIVIDE: return PLATFORM_KEYCODE_KP_DIVIDE;
    case KEY_KP_MULTIPLY: return PLATFORM_KEYCODE_KP_MULTIPLY;
    case KEY_KP_SUBTRACT: return PLATFORM_KEYCODE_KP_SUBTRACT;
    case KEY_KP_ADD: return PLATFORM_KEYCODE_KP_ADD;
    case KEY_KP_ENTER: return PLATFORM_KEYCODE_KP_ENTER;
    case KEY_KP_EQUAL: return PLATFORM_KEYCODE_KP_EQUAL;
    case KEY_BACK: return PLATFORM_KEYCODE_BACK;
    case KEY_MENU: return PLATFORM_KEYCODE_MENU;
    case KEY_VOLUME_UP: return PLATFORM_KEYCODE_VOLUME_UP;
    case KEY_VOLUME_DOWN: return PLATFORM_KEYCODE_VOLUME_DOWN;
    default: return PLATFORM_KEYCODE_NONE;
    }

    return PLATFORM_KEYCODE_NONE;
}

MouseButton get_raylib_mouse_button(platform_mouse_button_t mouse_button) {
    switch(mouse_button) {
    case PLATFORM_MOUSE_BUTTON_LEFT : return MOUSE_BUTTON_LEFT;
    case PLATFORM_MOUSE_BUTTON_RIGHT: return MOUSE_BUTTON_RIGHT;
    case PLATFORM_MOUSE_BUTTON_MIDDLE: return MOUSE_BUTTON_MIDDLE;
    case PLATFORM_MOUSE_BUTTON_SIDE: return MOUSE_BUTTON_SIDE;
    case PLATFORM_MOUSE_BUTTON_EXTRA: return MOUSE_BUTTON_EXTRA;
    case PLATFORM_MOUSE_BUTTON_FORWARD: return MOUSE_BUTTON_FORWARD;
    case PLATFORM_MOUSE_BUTTON_BACK: return MOUSE_BUTTON_BACK;
    }

    return 0;
}

// TODO: implement this properly
GamepadButton gamepad_button_to_raylib_GamepadButton(platform_gamepad_button_t gamepad_button) {
    return gamepad_button;
}

// TODO: implement this properly
GamepadAxis gamepad_axis_to_raylib_GamepadAxis(platform_gamepad_axis_t gamepad_axis) {
    return gamepad_axis;
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

platform_camera_t raylib_Camera2D_to_camera(Camera2D camera) {
    return (platform_camera_t) {
        .offset = raylib_Vector2_to_vec2f(camera.offset),
        .target = raylib_Vector2_to_vec2f(camera.target),
        .rotation = camera.rotation,
        .zoom = camera.zoom
    };
}

Camera2D camera_to_raylib_Camera2D(platform_camera_t camera) {
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

platform_texture_t raylib_Texture2D_to_texture(Texture2D texture) {
    platform_texture_t result = {
        .id = texture.id,
        .width = texture.width,
        .height = texture.height
    };

    return result;
}

// NOTE: The engine assumes that all textures are PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 this is fine because we will use our own format but it would be safer to assert when that isn't the case
Texture2D texture_to_raylib_Texture2D(platform_texture_t texture) {
    Texture2D result = {
        .id = texture.id,
        .width = texture.width,
        .height = texture.height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
    };

    return result;
}

platform_sound_t raylib_Sound_to_sound(Sound sound) {
    platform_audio_stream_t audio_stream = {
        .buffer = sound.stream.buffer,
        .processor = sound.stream.processor,
        .sample_rate = sound.stream.sampleRate,
        .sample_size = sound.stream.sampleSize,
        .channels = sound.stream.channels,
    };
    
    platform_sound_t result = {
        .audio_stream = audio_stream,
        .frame_count = sound.frameCount,
    };

    return result;
}

Sound sound_to_raylib_Sound(platform_sound_t sound) {
    AudioStream audio_stream = {
        .buffer = sound.audio_stream.buffer,
        .processor = sound.audio_stream.processor,
        .sampleRate = sound.audio_stream.sample_rate,
        .sampleSize = sound.audio_stream.sample_size,
        .channels = sound.audio_stream.channels,
    };
    
    Sound result = {
        .stream = audio_stream,
        .frameCount = sound.frame_count,
    };

    return result;
}

RenderTexture2D frame_buffer_to_raylibe_RenderTexture2D(platform_frame_buffer_t frame_buffer) {
    RenderTexture2D result = {
        .id = frame_buffer.id,
        .texture = texture_to_raylib_Texture2D(frame_buffer.texture),
        .depth = (Texture) {0},
    };

    return result;
}

size_t text_lenght(const char *text) {
    size_t result = 0;
    
    if (text == NULL) {
        return 0;
    }

    while (text[result] != 0) {
        result += 1;
    }

    return result;
}

platform_codepoint_t get_next_codepoint(const char *text, size_t *out_codepoint_size) {
    const char *ptr = text;
    platform_codepoint_t codepoint = 0x3f;
    size_t codepoint_size = 0;

    if (0xf0 == (0xf8 & ptr[0])) {
        if (!(((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80) || ((ptr[3] & 0xC0) ^ 0x80))) {
            codepoint = ((0x07 & ptr[0]) << 18) | ((0x3f & ptr[1]) << 12) | ((0x3f & ptr[2]) << 6) | (0x3f & ptr[3]);
            codepoint_size = 4;
        }
    } else if (0xe0 == (0xf0 & ptr[0])) {
        if (!(((ptr[1] & 0xC0) ^ 0x80) || ((ptr[2] & 0xC0) ^ 0x80))) {
            codepoint = ((0x0f & ptr[0]) << 12) | ((0x3f & ptr[1]) << 6) | (0x3f & ptr[2]);
            codepoint_size = 3;
        }
    } else if (0xc0 == (0xe0 & ptr[0])) {
        if (!((ptr[1] & 0xC0) ^ 0x80)) {
            codepoint = ((0x1f & ptr[0]) << 6) | (0x3f & ptr[1]);
            codepoint_size = 2;
        }
    } else if (0x00 == (0x80 & ptr[0])) {
        codepoint = ptr[0];
        codepoint_size = 1;
    } else {
        codepoint = 0x3f;
        codepoint_size = 0;
    }

    if (out_codepoint_size != NULL) {
        *out_codepoint_size = codepoint_size;
    }

    return codepoint;
}

size_t get_code_point_index(platform_font_t font, platform_codepoint_t codepoint, bool *out_found) {
    size_t index = 0;
    bool found = false;

    const platform_codepoint_t fallback_codepoint = '?';

    if (platform_is_font_valid(font)) {
        size_t fallback = 0;
        
        for (size_t i = 0; i < font.glyph_count; i++) {
            if (font.glyphs[i].codepoint == fallback_codepoint) {
                fallback = i;
            }

            if (font.glyphs[i].codepoint == codepoint) {
                index = i;
                found = true;
            }
        }

        if (!found) {
            index = fallback;
        }
    }

    if (out_found != NULL) {
        *out_found = found;
    }

    return index;
}