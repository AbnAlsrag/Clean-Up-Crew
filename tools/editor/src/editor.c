#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>
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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(0, 0, "CUC Editor");
    MaximizeWindow();
    SetTargetFPS(60);

    bool x = false;
    size_t active_tab = 0;
    imgui_string_t tabs[] = {
        imgui_String("map editor"),
        imgui_String("entity editor"),
        imgui_String("resource manger"),
        imgui_String("localization editor")
    };
    imgui_string_t contex_items[] = {
        imgui_String("create room"),
    };
    Color color;
    float value = 0.0f;
    float value2 = 0.0f;
    char text_data[100] = {0};
    size_t contex_item = IMGUI_UNLIMITED;
    bool room_menu = false;

    while (!WindowShouldClose()) {
        // printf("value = %f\n", value);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (active_tab == MAP_EDITOR_INDEX) {
                draw_map_editor();
            }
            if (room_menu) {
                contex_item = imgui_contex_menu(tabs, sizeof(tabs)/sizeof(tabs[0]));
                if (contex_item == 1) {
                    room_menu = false;
                }
            } else {
                imgui_rect((imgui_rect_t) { 50, 300, 350, 50 }, 5.0f, IMGUI_COLOR_BLACK, IMGUI_COLOR_RED);
                contex_item = imgui_contex_menu(contex_items, sizeof(contex_items)/sizeof(contex_items[0]));

                if (contex_item == 0) {
                    room_menu = true;
                }
                // value = imgui_slider((imgui_rect_t) { 50, 60, 350, 50 }, imgui_String("0"), imgui_String("10"), value, 0, 10);
                // GuiSliderBar((Rectangle) { 50, 150, 150, 50 }, "0", "10", &value, 0, 10);
                // GuiSliderBar((Rectangle) { 50, 150, 250, 50 }, "0", "10", &value2, 0, 10);
                // if (GuiTextBox((Rectangle) { 50, 150, 250, 50 }, text_data, 100, x)) x = true;
            }
            active_tab = imgui_tab_menu((imgui_rect_t) { 0, 0, GetScreenWidth(), 50 }, tabs, sizeof(tabs)/sizeof(tabs[0]), active_tab);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}