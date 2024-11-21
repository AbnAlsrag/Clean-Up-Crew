#include "renderer.h"
#include "platform/platform.h"

// renderer_t renderer = {0};
// Font font = {0};

texture_t player;

void init_render_contex(void) {
    platform_config_t config = {
        .width = 0, .height = 0,
        .title = "CUC",
        .target_fps = 60,
        .config_flags = CONFIG_FLAG_BORDERLESS_WINDOWED | CONFIG_FLAG_VSYNC,
    };

    platform_init(config);

    player = load_texture("resources/imgs/test.png");

    // SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT);
    // SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    // SetExitKey(KEY_NULL);
    // SetTargetFPS(60);
    // renderer.screen_width = GetScreenWidth();
    // renderer.screen_height = GetScreenHeight();

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

// viewport_t create_viewport(player_t *player) {
//     viewport_t viewport;

//     viewport.player = player;
//     viewport.camera = (Camera2D) { 
//         .offset = (vec2f_t) { .x = 0.0f, .y = 0.0f },
//         .target = player->position,
//         .rotation = 0, .zoom = 1.0f };
//     viewport.rect = (rectf_t) { 0.0f, 0.0f, 0.0f, 0.0f };
//     // viewport.fb = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

//     return viewport;
// }

// void compute_viewport_rects(void) {
//     int width = renderer.screen_width;
//     int height = renderer.screen_height;
    
//     switch (renderer.viewport_count) {
//     case 0: {
//         TraceLog(LOG_ERROR, "There is no active viewports atleast one is needed");
//     } break;
//     case 1: {
//         width = renderer.screen_width;
//         height = renderer.screen_height;
//         renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
//     } break;
//     case 2: {
//         width = width/2;
//         height = renderer.screen_height;
//         renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
//         renderer.viewports[1].rect = (rectf_t) { width, 0, width, height };
//     } break;
//     case 3: {
//         // width = width/2;
//         // height = height/2;
//         // renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
//         // renderer.viewports[1].rect = (rectf_t) { width, 0, width, height };
//         // // renderer.viewports[2].rect = (rectf_t) { 0, height, width*2, height };
//         // renderer.viewports[2].rect = (rectf_t) { 0, height, width, height };

//         width = width/3;
//         height = height;
//         renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
//         renderer.viewports[1].rect = (rectf_t) { width, 0, width, height };
//         renderer.viewports[2].rect = (rectf_t) { width*2, 0, width, height };
//     } break;
//     case 4: {
//         width = width/2;
//         height = height/2;
//         renderer.viewports[0].rect = (rectf_t) { 0, 0, width, height };
//         renderer.viewports[1].rect = (rectf_t) { width, 0, width, height };
//         renderer.viewports[2].rect = (rectf_t) { 0, height, width, height };
//         renderer.viewports[3].rect = (rectf_t) { width, height, width, height };
//     } break;
//     }

//     for (size_t i = 0; i < renderer.viewport_count; i++) {
//         int fb_width = renderer.viewports[i].rect.width;
//         int fb_height = renderer.viewports[i].rect.height;
        
//         if (IsRenderTextureReady(renderer.viewports[i].fb)) {
//             UnloadRenderTexture(renderer.viewports[i].fb);
//         }

//         renderer.viewports[i].fb = LoadRenderTexture(fb_width, fb_height);
//     }
// }

// bool renderer_register_viewport(viewport_t viewport, uint8_t *out_id) {
//     if (renderer.viewport_count >= MAX_VIEWPORTS) {
//         TraceLog(LOG_WARNING, "Max viewports reached can't register the given viewport");
//         return false;
//     }

//     renderer.viewports[renderer.viewport_count] = viewport;

//     if (out_id != NULL) {
//         *out_id = renderer.viewport_count;
//     }
    
//     renderer.viewport_count += 1;

//     compute_viewport_rects();

//     return true;
// }

// bool renderer_unregister_viewport(uint8_t id) {
//     if (id >= renderer.viewport_count) {
//         TraceLog(LOG_WARNING, "Invalid viewport id can't unregister an invalid id");
//         TraceLog(LOG_DEBUG, TextFormat("viewport_count = %d, id = %d", renderer.viewport_count, id));
//         return false;
//     }

//     renderer.viewport_count -= 1;
//     for (size_t i = id; i < renderer.viewport_count; i++) {
//         renderer.viewports[i] = renderer.viewports[i+1];
//     }

//     compute_viewport_rects();

//     return true;
// }

#include <raylib/raylib.h>

void renderer_update(void) {
    begin_drawing();
        clear_background(COLOR_RAYWHITE);
        draw_texture(player, (rectf_t) { 0, 0, player.width, player.height }, (rectf_t) { 0, 0, player.width, player.height }, VECTOR2_ZERO, 0.0f, get_color(0xFF0000FF));
    end_drawing();
//     {
//         int screen_width = GetScreenWidth();
//         int screen_height = GetScreenHeight();

//         if (renderer.screen_width != screen_width || renderer.screen_height != screen_height) {
//             renderer.screen_width = screen_width;
//             renderer.screen_height = screen_height;
//             compute_viewport_rects();
//         }
//     }

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
    unload_texture(player);
    platform_deinit();
}