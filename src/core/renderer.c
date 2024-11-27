#include "renderer.h"
#include "platform/platform.h"
#include "utils.h"

#include <raylib/raylib.h>
#include <raylib/rlgl.h>

#include <stdio.h>
#include <math.h>

renderer_t renderer = {0};

platform_texture_t player;
platform_sound_t sound;

void init_render_contex(void) {
    platform_config_t config = {
        .width = 0, .height = 0,
        .title = "CUC",
        .target_fps = 0,
        .config_flags = PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED | PLATFORM_CONFIG_FLAG_VSYNC*0,
    };

    platform_init(config);

    player = platform_load_texture("resources/imgs/test.png");
    sound = platform_load_sound("resources/sounds/test.wav");

    // SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT);
    // SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    // SetExitKey(KEY_NULL);
    // SetTargetFPS(60);
    // renderer.screen_width = GetScreenWidth();
    // renderer.screen_height = GetScreenHeight();

    renderer.screen_width = platform_get_window_size().x;
    renderer.screen_height = platform_get_window_size().y;

    // #define CODEPOINTS_SIZE 1024
    // static int codepoints[CODEPOINTS_SIZE];
    // (void)codepoints;

    // size_t idx = 0;
    // size_t i = 0;
    // for (idx = 0x0400; idx < 0x04FF && i < CODEPOINTS_SIZE; i++) {
        // codepoints[i] = idx;
        // idx += 1;
    // }

    // for (idx = 0x0600; idx < 0x06FF && i < CODEPOINTS_SIZE; i++) {
        // codepoints[i] = idx;
        // idx += 1;
    // }

    // for (idx = 0xFE70; idx < 0xFEFF && i < CODEPOINTS_SIZE; i++) {
        // codepoints[i] = idx;
        // idx += 1;
    // }

    // for (idx = 0x0000; idx < 0x007F && i < CODEPOINTS_SIZE; i++) {
        // codepoints[i] = idx;
        // idx += 1;
    // }

    // font = LoadFontEx("resources/fonts/DejaVuSans.ttf", 64, codepoints, i);
}

void compute_splitscreen_rects(void) {
    int width = renderer.screen_width;
    int height = renderer.screen_height;

    switch (renderer.splitscreen_count) {
    case 1: {
        width = renderer.screen_width;
        height = renderer.screen_height;

        renderer.splitscreens[0].viewport.x = 0;
        renderer.splitscreens[0].viewport.y = 0;
        renderer.splitscreens[0].viewport.width = width;
        renderer.splitscreens[0].viewport.height = height;
    } break;
    case 2: {
        width = width/2;
        height = renderer.screen_height;

        renderer.splitscreens[0].viewport.x = 0;
        renderer.splitscreens[0].viewport.y = 0;
        renderer.splitscreens[0].viewport.width = width;
        renderer.splitscreens[0].viewport.height = height;

        renderer.splitscreens[1].viewport.x = width;
        renderer.splitscreens[1].viewport.y = 0;
        renderer.splitscreens[1].viewport.width = width;
        renderer.splitscreens[1].viewport.height = height;
    } break;
    case 3: {
        width = width/3;
        height = height;

        renderer.splitscreens[0].viewport.x = 0;
        renderer.splitscreens[0].viewport.y = 0;
        renderer.splitscreens[0].viewport.width = width;
        renderer.splitscreens[0].viewport.height = height;

        renderer.splitscreens[1].viewport.x = width;
        renderer.splitscreens[1].viewport.y = 0;
        renderer.splitscreens[1].viewport.width = width;
        renderer.splitscreens[1].viewport.height = height;

        renderer.splitscreens[2].viewport.x = width*2;
        renderer.splitscreens[2].viewport.y = 0;
        renderer.splitscreens[2].viewport.width = width;
        renderer.splitscreens[2].viewport.height = height;
    } break;
    case 4: {
        width = width/2;
        height = height/2;

        renderer.splitscreens[0].viewport.x = 0;
        renderer.splitscreens[0].viewport.y = 0;
        renderer.splitscreens[0].viewport.width = width;
        renderer.splitscreens[0].viewport.height = height;

        renderer.splitscreens[1].viewport.x = width;
        renderer.splitscreens[1].viewport.y = 0;
        renderer.splitscreens[1].viewport.width = width;
        renderer.splitscreens[1].viewport.height = height;

        renderer.splitscreens[2].viewport.x = 0;
        renderer.splitscreens[2].viewport.y = height;
        renderer.splitscreens[2].viewport.width = width;
        renderer.splitscreens[2].viewport.height = height;

        renderer.splitscreens[3].viewport.x = width;
        renderer.splitscreens[3].viewport.y = height;
        renderer.splitscreens[3].viewport.width = width;
        renderer.splitscreens[3].viewport.height = height;
    } break;
    }

}

bool renderer_register_splitscreen(vec2f_t *player_position, uint8_t *out_id) {
    if (player_position == NULL) {
        return false;
    }

    if (renderer.splitscreen_count >= MAX_SPLITSCREENS) {
        return false;
    }

    splitscreen_t splitscreen = {0};
    splitscreen.player_position = player_position;
    splitscreen.camera.zoom = 1.0f;

    renderer.splitscreens[renderer.splitscreen_count] = splitscreen;

    if (out_id != NULL) {
        *out_id = renderer.splitscreen_count;
    }

    renderer.splitscreen_count += 1;

    compute_splitscreen_rects();

    return true;
}

bool renderer_unregister_splitscreen(uint8_t id) {
    if (id >= renderer.splitscreen_count) {
        return false;
    }

    renderer.splitscreen_count -= 1;
    for (size_t i = id; i < renderer.splitscreen_count; i++) {
        renderer.splitscreens[i] = renderer.splitscreens[i+1];
    }

    compute_splitscreen_rects();

    return true;
}

void renderer_update(void) {
    if (platform_was_window_resized()) {
        renderer.screen_width = platform_get_window_size().x;
        renderer.screen_height = platform_get_window_size().y;
        compute_splitscreen_rects();
    }

    platform_begin_drawing();
    for (size_t i = 0; i < renderer.splitscreen_count; i++) {
        splitscreen_t *splitscreen = &renderer.splitscreens[i];
        uint32_t width = splitscreen->viewport.width;
        uint32_t height = splitscreen->viewport.height;
        vec2f_t pos = (vec2f_t) { splitscreen->player_position->x, splitscreen->player_position->y };
        float lerp_factor = 1.0f - exp(-5.0 * platform_get_delta_time());
        splitscreen->camera.target = lerpfv2(splitscreen->camera.target, pos, lerp_factor);
        splitscreen->camera.offset = (vec2f_t) { width/2.0f, height/2.0f };

        platform_begin_viewport(splitscreen->viewport);
        platform_begin_camera(splitscreen->camera);
            platform_clear_background(COLOR_BLACK);
            if (i == 0) {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_RED);
            } else {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_WHITE);
            }

            platform_draw_rect((rectf_t) { splitscreen->player_position->x, splitscreen->player_position->y, 100, 100 }, VECTOR2_ZERO, 0.0f, COLOR_PURPLE);
        
        platform_end_camera();
        platform_end_viewport();
    }
        DrawFPS(10, 10);
    platform_end_drawing();
}

void deinit_render_contex(void) {
    platform_unload_texture(player);
    platform_unload_sound(sound);
    platform_deinit();
}