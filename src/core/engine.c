#include "engine.h"

#include <math.h>

static cuc_engine_t engine = {0};

void compute_splitscreen_rects(void);

void cuc_engine_init(void) {
    platform_config_t config = {
        .width = 0, .height = 0,
        .title = "CUC",
        .target_fps = 0,
        .config_flags = PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED | PLATFORM_CONFIG_FLAG_VSYNC*0,
    };

    platform_init(config);

    engine.screen_width = platform_get_window_size().x;
    engine.screen_height = platform_get_window_size().y;
}

void cuc_engine_deinit(void) {
    platform_deinit();
}

bool cuc_engine_register_splitscreen(player_t *player, uint8_t *out_id) {
    if (player == NULL) {
        return false;
    }

    if (engine.splitscreen_count >= MAX_SPLITSCREENS) {
        return false;
    }

    splitscreen_t splitscreen = {0};
    splitscreen.player = player;
    splitscreen.camera.zoom = 1.0f;

    engine.splitscreens[engine.splitscreen_count] = splitscreen;

    if (out_id != NULL) {
        *out_id = engine.splitscreen_count;
    }

    engine.splitscreen_count += 1;

    compute_splitscreen_rects();

    return true;
}
bool cuc_engine_unregister_splitscreen(uint8_t id) {
    if (id >= engine.splitscreen_count) {
        return false;
    }

    engine.splitscreen_count -= 1;
    for (size_t i = id; i < engine.splitscreen_count; i++) {
        engine.splitscreens[i] = engine.splitscreens[i+1];
    }

    compute_splitscreen_rects();

    return true;
}

void cuc_engine_update(void) {
    if (platform_was_window_resized()) {
        engine.screen_width = platform_get_window_size().x;
        engine.screen_height = platform_get_window_size().y;
        compute_splitscreen_rects();
    }

    platform_begin_drawing();
    for (size_t i = 0; i < engine.splitscreen_count; i++) {
        splitscreen_t *splitscreen = &engine.splitscreens[i];
        uint32_t width = splitscreen->viewport.width;
        uint32_t height = splitscreen->viewport.height;
        vec2f_t pos = (vec2f_t) { splitscreen->player->pos.x, splitscreen->player->pos.y };
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

            platform_draw_rect((rectf_t) { splitscreen->player->pos.x, splitscreen->player->pos.y, 100, 100 }, VECTOR2_ZERO, 0.0f, COLOR_PURPLE);
        
        platform_end_camera();
        platform_end_viewport();
    }
        // DrawFPS(10, 10);
    platform_end_drawing();
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// INTERNAL /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void compute_splitscreen_rects(void) {
    int width = engine.screen_width;
    int height = engine.screen_height;

    switch (engine.splitscreen_count) {
    case 1: {
        width = engine.screen_width;
        height = engine.screen_height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;
    } break;
    case 2: {
        width = width/2;
        height = engine.screen_height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;
    } break;
    case 3: {
        width = width/3;
        height = height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;

        engine.splitscreens[2].viewport.x = width*2;
        engine.splitscreens[2].viewport.y = 0;
        engine.splitscreens[2].viewport.width = width;
        engine.splitscreens[2].viewport.height = height;
    } break;
    case 4: {
        width = width/2;
        height = height/2;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;

        engine.splitscreens[2].viewport.x = 0;
        engine.splitscreens[2].viewport.y = height;
        engine.splitscreens[2].viewport.width = width;
        engine.splitscreens[2].viewport.height = height;

        engine.splitscreens[3].viewport.x = width;
        engine.splitscreens[3].viewport.y = height;
        engine.splitscreens[3].viewport.width = width;
        engine.splitscreens[3].viewport.height = height;
    } break;
    }
}