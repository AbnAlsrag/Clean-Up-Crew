#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>

#define IMGUI_IMPLEMENTATION
#include "imgui.h"

typedef struct room_t {
    imgui_rect_t rect;
} room_t;

#define MAX_ROOMS 50
#define MAP_EDITOR_INDEX 0

Vector2 camera_pos = { 0, 0 };
Camera2D camera = { .zoom = 1.0f };
float zoom_factor = 0.2f;

void draw_map_editor(void) {
    BeginMode2D(camera);
    imgui_begin_camera_mode(imgui_Camera2D_to_camera(camera));
        {
            if (GetMouseWheelMove() != 0) {
                Vector2 mouse_worldpos_before_zoom = GetScreenToWorld2D(GetMousePosition(), camera);

                camera.zoom += GetMouseWheelMove() * zoom_factor;
                camera.zoom = Clamp(camera.zoom, 0.1f, 10.0f);

                Vector2 mouse_worldpos_after_zoom = GetScreenToWorld2D(GetMousePosition(), camera);

                Vector2 diff = Vector2Subtract(mouse_worldpos_after_zoom, mouse_worldpos_before_zoom);
                camera_pos = Vector2Subtract(camera_pos, diff);
            }

            camera.target = camera_pos;

            if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
                Vector2 mouse_delta = GetMouseDelta();
                camera_pos = Vector2Subtract(camera_pos, Vector2Scale(mouse_delta, 1.0f/camera.zoom));
            }
        }

        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), RED);
        imgui_toggle_button((imgui_rect_t) { 50, 50, 100, 50 }, imgui_String("MNS R5M"), false);
    imgui_end_camera_mode();
    EndMode2D();
}

int main(void) {
    InitWindow(0, 0, "CUC Editor");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}