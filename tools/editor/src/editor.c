#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raygui.h>
#include <raylib/raylib.h>

typedef enum legend_kind_t {
    LEGEND_KIND_NONE = 0,
    LEGEND_KIND_ROOM,
    MAX_LEGEND_KIND,
} legend_kind_t;

typedef struct room_t {
    Rectangle rect;
} room_t;

#define MAX_ROOMS 50

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 650, "CUC Editor");
    SetTargetFPS(60);

    int active_tab = 0;
    const char *tabs[] = { "map editor", "entity editor", "localization editor" };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}