#include <raylib/raylib.h>
#define RRR_IMPLEMENTATION
#include "rrr.h"

rrr_color_t img_data[500*500] = {0};

Image to_image(rrr_color_t *pixels, rrr_u32_t width, rrr_u32_t height) {
    Image img = {
        .data = pixels,
        .width = width,
        .height = height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    return img;
}

int main(void) {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(500, 500, "test");

    SetTargetFPS(60);

    rrr_context_t context = rrr_create_context();
    rrr_make_context_current(context);

    rrr_canvas_t canvas = rrr_create_canvas(img_data, 500, 500, 500);

    Texture2D texture = LoadTextureFromImage(to_image(img_data, 500, 500));

    float x = 0.0f;
    float y = 0.0f;

    while (!WindowShouldClose()) {
        if (IsKeyDown(KEY_W)) {
            y -= 5.0f;
        } else if (IsKeyDown(KEY_S)) {
            y += 5.0f;
        }

        if (IsKeyDown(KEY_A)) {
            x -= 5.0f;
        } else if (IsKeyDown(KEY_D)) {
            x += 5.0f;
        }
        
        rrr_canvas_fill(canvas, RRR_COLOR_RED);
        rrr_canvas_draw_rect(canvas, (rrr_rect_t) { x, y, 50, 50 }, RRR_COLOR_DARKBLUE);
        // rrr_canvas_draw_circle(canvas, (rrr_circle_t) { .center = { x, y }, 50}, RRR_COLOR_DARKBLUE);

        BeginDrawing();
            UpdateTexture(texture, img_data);
            DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}