#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "core/cuc_utils.h"

#include <stdint.h>
#include <stdbool.h>

typedef enum platform_config_flags_t {
    PLATFORM_CONFIG_FLAG_VSYNC = 0x00000001,
    PLATFORM_CONFIG_FLAG_FULLSCREEN = 0x00000002,
    PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED = 0x00000004,
    PLATFORM_CONFIG_FLAG_RESIZABLE = 0x00000008,
} platform_config_flags_t;

typedef struct platform_config_t {
    uint32_t width;
    uint32_t height;
    char *title;
    uint32_t target_fps;
    platform_config_flags_t config_flags;
} platform_config_t;

typedef enum platform_keycode_t {
    PLATFORM_KEYCODE_NONE = 0,
    PLATFORM_KEYCODE_APOSTROPHE,          // Key: '
    PLATFORM_KEYCODE_COMMA,               // Key: ,
    PLATFORM_KEYCODE_MINUS,               // Key: -
    PLATFORM_KEYCODE_PERIOD,              // Key: .
    PLATFORM_KEYCODE_SLASH,               // Key: /
    PLATFORM_KEYCODE_ZERO,                // Key: 0
    PLATFORM_KEYCODE_ONE,                 // Key: 1
    PLATFORM_KEYCODE_TWO,                 // Key: 2
    PLATFORM_KEYCODE_THREE,               // Key: 3
    PLATFORM_KEYCODE_FOUR,                // Key: 4
    PLATFORM_KEYCODE_FIVE,                // Key: 5
    PLATFORM_KEYCODE_SIX,                 // Key: 6
    PLATFORM_KEYCODE_SEVEN,               // Key: 7
    PLATFORM_KEYCODE_EIGHT,               // Key: 8
    PLATFORM_KEYCODE_NINE,                // Key: 9
    PLATFORM_KEYCODE_SEMICOLON,           // Key: ;
    PLATFORM_KEYCODE_EQUAL,               // Key: =
    PLATFORM_KEYCODE_A,                   // Key: A | a
    PLATFORM_KEYCODE_B,                   // Key: B | b
    PLATFORM_KEYCODE_C,                   // Key: C | c
    PLATFORM_KEYCODE_D,                   // Key: D | d
    PLATFORM_KEYCODE_E,                   // Key: E | e
    PLATFORM_KEYCODE_F,                   // Key: F | f
    PLATFORM_KEYCODE_G,                   // Key: G | g
    PLATFORM_KEYCODE_H,                   // Key: H | h
    PLATFORM_KEYCODE_I,                   // Key: I | i
    PLATFORM_KEYCODE_J,                   // Key: J | j
    PLATFORM_KEYCODE_K,                   // Key: K | k
    PLATFORM_KEYCODE_L,                   // Key: L | l
    PLATFORM_KEYCODE_M,                   // Key: M | m
    PLATFORM_KEYCODE_N,                   // Key: N | n
    PLATFORM_KEYCODE_O,                   // Key: O | o
    PLATFORM_KEYCODE_P,                   // Key: P | p
    PLATFORM_KEYCODE_Q,                   // Key: Q | q
    PLATFORM_KEYCODE_R,                   // Key: R | r
    PLATFORM_KEYCODE_S,                   // Key: S | s
    PLATFORM_KEYCODE_T,                   // Key: T | t
    PLATFORM_KEYCODE_U,                   // Key: U | u
    PLATFORM_KEYCODE_V,                   // Key: V | v
    PLATFORM_KEYCODE_W,                   // Key: W | w
    PLATFORM_KEYCODE_X,                   // Key: X | x
    PLATFORM_KEYCODE_Y,                   // Key: Y | y
    PLATFORM_KEYCODE_Z,                   // Key: Z | z
    PLATFORM_KEYCODE_LEFT_BRACKET,        // Key: [
    PLATFORM_KEYCODE_BACKSLASH,           // Key: '\'
    PLATFORM_KEYCODE_RIGHT_BRACKET,       // Key: ]
    PLATFORM_KEYCODE_GRAVE,               // Key: `
    PLATFORM_KEYCODE_SPACE,               // Key: Space
    PLATFORM_KEYCODE_ESCAPE,              // Key: Esc
    PLATFORM_KEYCODE_ENTER,               // Key: Enter
    PLATFORM_KEYCODE_TAB,                 // Key: Tab
    PLATFORM_KEYCODE_BACKSPACE,           // Key: Backspace
    PLATFORM_KEYCODE_INSERT,              // Key: Ins
    PLATFORM_KEYCODE_DELETE,              // Key: Del
    PLATFORM_KEYCODE_RIGHT,               // Key: Cursor right
    PLATFORM_KEYCODE_LEFT,                // Key: Cursor left
    PLATFORM_KEYCODE_DOWN,                // Key: Cursor down
    PLATFORM_KEYCODE_UP,                  // Key: Cursor up
    PLATFORM_KEYCODE_PAGE_UP,             // Key: Page up
    PLATFORM_KEYCODE_PAGE_DOWN,           // Key: Page down
    PLATFORM_KEYCODE_HOME,                // Key: Home
    PLATFORM_KEYCODE_END,                 // Key: End
    PLATFORM_KEYCODE_CAPS_LOCK,           // Key: Caps lock
    PLATFORM_KEYCODE_SCROLL_LOCK,         // Key: Scroll down
    PLATFORM_KEYCODE_NUM_LOCK,            // Key: Num lock
    PLATFORM_KEYCODE_PRINT_SCREEN,        // Key: Print screen
    PLATFORM_KEYCODE_PAUSE,               // Key: Pause
    PLATFORM_KEYCODE_F1,                  // Key: F1
    PLATFORM_KEYCODE_F2,                  // Key: F2
    PLATFORM_KEYCODE_F3,                  // Key: F3
    PLATFORM_KEYCODE_F4,                  // Key: F4
    PLATFORM_KEYCODE_F5,                  // Key: F5
    PLATFORM_KEYCODE_F6,                  // Key: F6
    PLATFORM_KEYCODE_F7,                  // Key: F7
    PLATFORM_KEYCODE_F8,                  // Key: F8
    PLATFORM_KEYCODE_F9,                  // Key: F9
    PLATFORM_KEYCODE_F10,                 // Key: F10
    PLATFORM_KEYCODE_F11,                 // Key: F11
    PLATFORM_KEYCODE_F12,                 // Key: F12
    PLATFORM_KEYCODE_LEFT_SHIFT,          // Key: Shift left
    PLATFORM_KEYCODE_LEFT_CONTROL,        // Key: Control left
    PLATFORM_KEYCODE_LEFT_ALT,            // Key: Alt left
    PLATFORM_KEYCODE_LEFT_SUPER,          // Key: Super left
    PLATFORM_KEYCODE_RIGHT_SHIFT,         // Key: Shift right
    PLATFORM_KEYCODE_RIGHT_CONTROL,       // Key: Control right
    PLATFORM_KEYCODE_RIGHT_ALT,           // Key: Alt right
    PLATFORM_KEYCODE_RIGHT_SUPER,         // Key: Super right
    PLATFORM_KEYCODE_KB_MENU,             // Key: KB menu
    PLATFORM_KEYCODE_KP_0,                // Key: Keypad 0
    PLATFORM_KEYCODE_KP_1,                // Key: Keypad 1
    PLATFORM_KEYCODE_KP_2,                // Key: Keypad 2
    PLATFORM_KEYCODE_KP_3,                // Key: Keypad 3
    PLATFORM_KEYCODE_KP_4,                // Key: Keypad 4
    PLATFORM_KEYCODE_KP_5,                // Key: Keypad 5
    PLATFORM_KEYCODE_KP_6,                // Key: Keypad 6
    PLATFORM_KEYCODE_KP_7,                // Key: Keypad 7
    PLATFORM_KEYCODE_KP_8,                // Key: Keypad 8
    PLATFORM_KEYCODE_KP_9,                // Key: Keypad 9
    PLATFORM_KEYCODE_KP_DECIMAL,          // Key: Keypad .
    PLATFORM_KEYCODE_KP_DIVIDE,           // Key: Keypad /
    PLATFORM_KEYCODE_KP_MULTIPLY,         // Key: Keypad *
    PLATFORM_KEYCODE_KP_SUBTRACT,         // Key: Keypad -
    PLATFORM_KEYCODE_KP_ADD,              // Key: Keypad +
    PLATFORM_KEYCODE_KP_ENTER,            // Key: Keypad Enter
    PLATFORM_KEYCODE_KP_EQUAL,            // Key: Keypad =
    PLATFORM_KEYCODE_BACK,                // Key: Android back button
    PLATFORM_KEYCODE_MENU,                // Key: Android menu button
    PLATFORM_KEYCODE_VOLUME_UP,           // Key: Android volume up button
    PLATFORM_KEYCODE_VOLUME_DOWN,         // Key: Android volume down button
} platform_keycode_t;

typedef uint16_t gamepad_id_t;

typedef enum platform_mouse_button_t {
    PLATFORM_MOUSE_BUTTON_LEFT = 0,      // Mouse button left
    PLATFORM_MOUSE_BUTTON_RIGHT,         // Mouse button right
    PLATFORM_MOUSE_BUTTON_MIDDLE,        // Mouse button middle (pressed wheel)
    PLATFORM_MOUSE_BUTTON_SIDE,          // Mouse button side (advanced mouse device)
    PLATFORM_MOUSE_BUTTON_EXTRA,         // Mouse button extra (advanced mouse device)
    PLATFORM_MOUSE_BUTTON_FORWARD,       // Mouse button forward (advanced mouse device)
    PLATFORM_MOUSE_BUTTON_BACK,          // Mouse button back (advanced mouse device)
} platform_mouse_button_t;

typedef enum platform_gamepad_button_t {
    PLATFORM_GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
    PLATFORM_GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
    PLATFORM_GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
    PLATFORM_GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
    PLATFORM_GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
    PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
    PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Circle, Xbox: B)
    PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
    PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Square, Xbox: X)
    PLATFORM_GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
    PLATFORM_GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
    PLATFORM_GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (first), it could be a trailing button
    PLATFORM_GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
    PLATFORM_GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
    PLATFORM_GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
    PLATFORM_GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
    PLATFORM_GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
    PLATFORM_GAMEPAD_BUTTON_RIGHT_THUMB,         // Gamepad joystick pressed button right
} platform_gamepad_button_t;

typedef enum platform_gamepad_axis_t {
    PLATFORM_GAMEPAD_AXIS_LEFT_X = 0,            // Gamepad left stick X axis
    PLATFORM_GAMEPAD_AXIS_LEFT_Y,                // Gamepad left stick Y axis
    PLATFORM_GAMEPAD_AXIS_RIGHT_X,               // Gamepad right stick X axis
    PLATFORM_GAMEPAD_AXIS_RIGHT_Y,               // Gamepad right stick Y axis
    PLATFORM_GAMEPAD_AXIS_LEFT_TRIGGER,          // Gamepad back trigger left, pressure level: [1..-1]
    PLATFORM_GAMEPAD_AXIS_RIGHT_TRIGGER,         // Gamepad back trigger right, pressure level: [1..-1]
} platform_gamepad_axis_t;

typedef struct platform_audio_stream_t {
    void *buffer;
    void *processor;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t sample_size;
} platform_audio_stream_t;

typedef struct platform_sound_t {
    platform_audio_stream_t audio_stream;
    uint32_t frame_count;
} platform_sound_t;

typedef struct platform_viewport_t {
    uint32_t x, y;
    uint32_t width, height;
} platform_viewport_t;

typedef struct platform_camera_t {
    vec2f_t offset;
    vec2f_t target;
    float rotation;
    float zoom;
} platform_camera_t;

typedef enum platform_blend_mode_t {
    BLEND_MODE_ALPHA = 0,
    BLEND_MODE_ADDITIVE,
    BLEND_MODE_MULTIPLIED,
    BLEND_MODE_ADD_COLORS,
    BLEND_MODE_SUBTRACT_COLORS,
    BLEND_MODE_ALPHA_PREMULTIPLY,
} platform_blend_mode_t;

typedef struct platform_texture_t {
    uint32_t id;
    uint32_t width;
    uint32_t height;
} platform_texture_t;

typedef struct platform_frame_buffer_t {
    uint32_t id;
    platform_texture_t texture;
} platform_frame_buffer_t;

void platform_init(platform_config_t config);
void platform_reconfig(platform_config_t config);
void platform_deinit(void);
bool platform_running(void);
bool platform_was_window_resized(void);
vec2f_t platform_get_window_size(void);
void *platform_get_native_window_handle(void);

// TODO: maybe add keyboard specific input like in gamepads idk

bool platform_is_key_pressed(platform_keycode_t keycode);
bool platform_is_key_down(platform_keycode_t keycode);
bool platform_is_key_repeat(platform_keycode_t keycode);
bool platform_is_key_released(platform_keycode_t keycode);
bool platform_is_key_up(platform_keycode_t keycode);
platform_keycode_t platform_get_pressed_key(void);

bool platform_is_mouse_button_pressed(platform_mouse_button_t mouse_button);
bool platform_is_mouse_button_down(platform_mouse_button_t mouse_button);
bool platform_is_mouse_button_released(platform_mouse_button_t mouse_button);
bool platform_is_mouse_button_up(platform_mouse_button_t mouse_button);
bool platform_is_mouse_on_screen(void);
vec2f_t platform_get_mouse_position(void);
void platform_set_mouse_position(vec2f_t position);
vec2f_t platform_get_mouse_delta(void);
vec2f_t platform_get_mouse_wheel(void);

bool platform_is_gamepad_present(gamepad_id_t id);
// const char *platform_get_gamepad_name(gamepad_id_t id);
bool platform_is_gamepad_button_pressed(gamepad_id_t id, platform_gamepad_button_t button);
bool platform_is_gamepad_button_down(gamepad_id_t id, platform_gamepad_button_t button);
bool platform_is_gamepad_button_released(gamepad_id_t id, platform_gamepad_button_t button);
bool platform_is_gamepad_button_up(gamepad_id_t id, platform_gamepad_button_t button);
// platform_gamepad_button_t platform_get_gamepad_button_pressed(gamepad_id_t id);
float platform_get_gamepad_axis(gamepad_id_t id, platform_gamepad_axis_t axis);

void platform_set_master_volume(float volume);
float platform_get_master_volume(void);
platform_sound_t platform_load_sound(char *file_path);
void platform_unload_sound(platform_sound_t sound);

void platform_play_sound(platform_sound_t sound);
void platform_stop_sound(platform_sound_t sound);
void platform_pause_sound(platform_sound_t sound);
void platform_resume_sound(platform_sound_t sound);
bool platform_is_sound_playing(platform_sound_t sound);
void platform_set_sound_volume(platform_sound_t sound, float volume);
void platform_set_sound_pitch(platform_sound_t sound, float pitch);
void platform_set_sound_pan(platform_sound_t sound, float pan);

void platform_clear_background(color_t color);
void platform_begin_drawing(void);
void platform_end_drawing(void);
void platform_begin_viewport(platform_viewport_t viewport);
void platform_end_viewport(void);
void platform_begin_frame_buffer(platform_frame_buffer_t frame_buffer);
void platform_end_frame_buffer(void);
void platform_begin_camera(platform_camera_t camera);
void platform_end_camera(void);
void platform_begin_blend_mode(platform_blend_mode_t mode);
void platform_end_blend_mode(void);
void platform_begin_scissor_mode(rectf_t rect);
void platform_end_scissor_mode(void);
void platform_begin_wireframe_mode(void);
void platform_end_wireframe_mode(void);
vec2f_t platform_get_world_to_screen(platform_camera_t camera, vec2f_t pos);
vec2f_t platform_get_screen_to_world(platform_camera_t camera, vec2f_t pos);

platform_texture_t platform_load_texture(const char *file_path);
bool platform_is_texture_valid(platform_texture_t texture);
void platform_unload_texture(platform_texture_t texture);
platform_frame_buffer_t platform_load_frame_buffer(uint32_t width, uint32_t height);
void platform_unload_frame_buffer(platform_frame_buffer_t frame_buffer);

void platform_draw_fps(vec2f_t pos);
void platform_draw_line(vec2f_t start, vec2f_t end, float thickness, color_t color);
void platform_draw_triangle(vec2f_t point0, vec2f_t point1, vec2f_t point2, color_t color);
void platform_draw_rect(rectf_t rect, vec2f_t origin, float rotation, color_t color);
void platform_draw_circle(vec2f_t center, float radius, color_t color);
void platform_draw_circle_sector(vec2f_t center, float radius, float start_angle, float end_angle, color_t color);
void platform_draw_ring(vec2f_t center, float outer_radius, float inner_radius, float start_angle, float end_angle, color_t color);
void platform_draw_texture(platform_texture_t texture, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint);
void platform_draw_frame_buffer(platform_frame_buffer_t frame_buffer, rectf_t source, rectf_t dest, vec2f_t origin, float rotation, color_t tint);

float platform_get_delta_time(void);
double platform_get_time(void);
uint32_t platform_get_fps(void);

void platform_open_url(char *url);

#endif // _PLATFORM_H_