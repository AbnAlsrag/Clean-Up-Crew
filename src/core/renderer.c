#include "renderer.h"
#include "platform/platform.h"
#include "utils.h"
#include <math.h>

renderer_t renderer = {0};

platform_texture_t player;
platform_sound_t sound;
platform_frame_buffer_t fbo;

void init_render_contex(void) {
    platform_config_t config = {
        .width = 0, .height = 0,
        .title = "CUC",
        .target_fps = 60,
        .config_flags = PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED | PLATFORM_CONFIG_FLAG_VSYNC,
    };

    platform_init(config);

    player = platform_load_texture("resources/imgs/test.png");
    sound = platform_load_sound("resources/sounds/test.wav");
    fbo = platform_load_frame_buffer(200, 200);

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
        // width = width/2;
        // height = height/2;
        // renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
        // renderer.viewports[1].rect = (rectf_t) { width, 0, width, height };
        // // renderer.viewports[2].rect = (rectf_t) { 0, height, width*2, height };
        // renderer.viewports[2].rect = (rectf_t) { 0, height, width, height };

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
// void compute_viewport_rects(void) {

//     for (size_t i = 0; i < renderer.viewport_count; i++) {
//         int fb_width = renderer.viewports[i].rect.width;
//         int fb_height = renderer.viewports[i].rect.height;
        
//         if (IsRenderTextureReady(renderer.viewports[i].fb)) {
//             UnloadRenderTexture(renderer.viewports[i].fb);
//         }

//         renderer.viewports[i].fb = LoadRenderTexture(fb_width, fb_height);
//     }
// }

bool renderer_register_splitscreen(vec2f_t *player_position, uint8_t *out_id) {
    if (player_position == NULL) {
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
    // if (platform_was_window_resized()) {
    //     renderer.screen_width = platform_get_window_size().x;
    //     renderer.screen_height = platform_get_window_size().y;
    //     compute_splitscreen_rects();
    // }

    platform_begin_drawing();
    platform_clear_background(COLOR_BLACK);
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
            if (i == 0) {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_RED);
            } else {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_WHITE);
            }

            platform_draw_rect((rectf_t) { splitscreen->player_position->x, splitscreen->player_position->y, 100, 100 }, VECTOR2_ZERO, 0.0f, COLOR_PURPLE);
        
        platform_end_camera();
        platform_end_viewport();
    }
    platform_end_drawing();

//     for (size_t i = 0; i < renderer.viewport_count; i++) {
//         viewport_t *viewport = &renderer.viewports[i];
//         int width = viewport->rect.width;
//         int height = viewport->rect.height;
//         vec2f_t pos = (vec2f_t) { (viewport->player->position.x+50), (viewport->player->position.y+50) };
//         float lerpFactor = 1.0f - exp(-5.0 * GetFrameTime());
//         viewport->camera.target = lerpfv2(viewport->camera.target, pos, lerpFactor);
//         viewport->camera.offset = (vec2f_t) { width/2, height/2 };
        
//         BeginTextureMode(viewport->fb);
//         BeginMode2D(viewport->camera);
//             ClearBackground(BLACK);
//             if (i == 0) {
//                 DrawRectangle(0, 0, width, height, RED);
//             } else {
//                 DrawRectangle(0, 0, width, height, WHITE);
//             }

//             // DrawText(TextFormat("target = "VECTOR2_FMT, VECTOR2_ARG(pos)), 0, 150, 16, GREEN);
//             // DrawText(TextFormat("camera = "VECTOR2_FMT, VECTOR2_ARG(viewport->camera.target)), 0, 100, 16, GREEN);
//             // DrawText(TextFormat("player = "VECTOR2_FMT, VECTOR2_ARG(viewport->player->position)), 0, 200, 16, GREEN);
//             // DrawText(TextFormat("camera_offset = "VECTOR2_FMT, VECTOR2_ARG(viewport->camera.offset)), 0, 250, 16, GREEN);
//             DrawRectangleV(viewport->player->position, (vec2f_t) { 100, 100 }, PURPLE);
//         EndMode2D();
//             // DrawText("hello world", 50, 50, 28, RED);
//         EndTextureMode();
//     }

//     BeginDrawing();
//         ClearBackground(BLACK);

//         for (size_t i = 0; i < renderer.viewport_count; i++) {
//             rectf_t source = (rectf_t) { 
//                 .x = 0,
//                 .y = 0,
//                 .width = renderer.viewports[i].fb.texture.width,
//                 .height = -renderer.viewports[i].fb.texture.height };
            
//             rectf_t dest = renderer.viewports[i].rect;
//             DrawTextureRec(renderer.viewports[i].fb.texture, source, (vec2f_t) { dest.x, dest.y }, WHITE);
//         }

//         // DrawTextEx(font, "АБН АЛСРАГ", (vec2f_t) { .x = 100, .y = 100 }, 64, 0.2, GREEN);
//         // DrawTextEx(font, "\uFEE2\uFEA7\uFEAD \uFEE5\uFE8D\uFEED\uFEAE\uFEE3", (vec2f_t) { .x = 100, .y = 150}, 64, 0.2, GREEN);
//         // DrawTextEx(font, "ABN ALSRAG", (vec2f_t) { .x = 100, .y = 230 }, 64, 0.2, GREEN);

//         DrawFPS(0, 0);
//     EndDrawing();
}

void deinit_render_contex(void) {
    platform_unload_texture(player);
    platform_unload_sound(sound);
    platform_deinit();
}