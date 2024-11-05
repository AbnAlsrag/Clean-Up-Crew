#include "renderer.h"

#include <math.h>
#include <raylib/raylib.h>

renderer_t renderer = {0};
Font font = {0};

void init_render_contex(void) {
    // const int screenWidth = 800;
    // const int screenHeight = 450;

    const int screenWidth = 0;
    const int screenHeight = 0;

    #if !ALLOW_LOGGING
    SetTraceLogLevel(LOG_NONE);
    #else
    SetTraceLogLevel(LOG_ALL);
    #endif
    
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(screenWidth, screenHeight, "Clean Up Crew");
    // SetTargetFPS(60);

    #define CODEPOINTS_SIZE 1024
    static int codepoints[CODEPOINTS_SIZE];
    (void)codepoints;

    size_t idx = 0;
    size_t i = 0;
    for (idx = 0x0400; idx < 0x04FF && i < CODEPOINTS_SIZE; i++) {
        codepoints[i] = idx;
        idx += 1;
    }

    for (idx = 0x0600; idx < 0x06FF && i < CODEPOINTS_SIZE; i++) {
        codepoints[i] = idx;
        idx += 1;
    }

    for (idx = 0xFE70; idx < 0xFEFF && i < CODEPOINTS_SIZE; i++) {
        codepoints[i] = idx;
        idx += 1;
    }

    for (idx = 0x0000; idx < 0x007F && i < CODEPOINTS_SIZE; i++) {
        codepoints[i] = idx;
        idx += 1;
    }

    font = LoadFontEx("resources/fonts/DejaVuSans.ttf", 64, codepoints, i);
}

viewport_t create_viewport(player_t *player) {
    viewport_t viewport;

    viewport.player = player;
    viewport.camera = (Camera2D) { 
        .offset = (vector2_t) { .x = 0.0f, .y = 0.0f },
        .target = player->position,
        .rotation = 0, .zoom = 1.0f };
    viewport.rect = (rectangle_t) { 0.0f, 0.0f, 0.0f, 0.0f };
    viewport.fb = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    return viewport;
}

bool renderer_register_viewport(viewport_t viewport) {
    if (renderer.viewport_count >= MAX_VIEWPORTS) {
        TraceLog(LOG_WARNING, "Max viewports reached can't register the given viewport");
        return false;
    }

    renderer.viewports[renderer.viewport_count] = viewport;
    renderer.viewport_count += 1;

    return true;
}

void renderer_update() {
    for (size_t i = 0; i < renderer.viewport_count; i++) {
        viewport_t *viewport = &renderer.viewports[i];
        int width = viewport->rect.width;
        int height = viewport->rect.height;
        vector2_t pos = (vector2_t) { (viewport->player->position.x+50), (viewport->player->position.y+50) };
        float lerpFactor = 1.0f - exp(-5.0 * GetFrameTime());
        viewport->camera.target = lerpfv2(viewport->camera.target, pos, lerpFactor);
        viewport->camera.offset = (vector2_t) { width/2, height/2 };
        BeginTextureMode(viewport->fb);
        BeginMode2D(viewport->camera);
            ClearBackground(BLACK);
            if (i == 0) {
                DrawRectangle(0, 0, width, height, RED);
            } else {
                DrawRectangle(0, 0, width, height, WHITE);
            }

            DrawRectangle(viewport->player->position.x, viewport->player->position.y, 100, 100, PURPLE);
        EndMode2D();
        EndTextureMode();
    }

    BeginDrawing();
        ClearBackground(BLACK);

        for (size_t i = 0; i < renderer.viewport_count; i++) {
            rectangle_t source = (rectangle_t) { 
                .x = 0,
                .y = 0,
                .width = renderer.viewports[i].rect.width,
                .height = -renderer.viewports[i].rect.height };
            
            rectangle_t dest = renderer.viewports[i].rect;
            DrawTextureRec(renderer.viewports[i].fb.texture, source, (vector2_t) { dest.x, dest.y }, WHITE);
        }

        // DrawTextEx(font, "АБН АЛСРАГ", (vector2_t) { .x = 100, .y = 100 }, 64, 0.2, GREEN);
        // DrawTextEx(font, "\uFEE2\uFEA7\uFEAD \uFEE5\uFE8D\uFEED\uFEAE\uFEE3", (vector2_t) { .x = 100, .y = 150}, 64, 0.2, GREEN);
        // DrawTextEx(font, "ABN ALSRAG", (vector2_t) { .x = 100, .y = 230 }, 64, 0.2, GREEN);

        DrawFPS(0, 0);
    EndDrawing();
}

void deinit_render_contex(void) {
    UnloadFont(font);
    for (size_t i = 0; i < renderer.viewport_count; i++) {
        UnloadRenderTexture(renderer.viewports[i].fb);
    }
    CloseWindow();
}