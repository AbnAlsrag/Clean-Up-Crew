#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAYGUI_IMPLEMENTATION
#include <raylib/raylib.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>

#define IMGUI_IMPLEMENTATION
#include "imgui.h"

#define ILLEGAL_ROOM_ID ((room_id_t)-1)
#define MAX_ROOMS 50
#define ROOM_SLOTS_SIZE ((MAX_ROOMS+7)/8)
#define DEFAULT_ROOM_WIDTH 100
#define DEFAULT_ROOM_HEIGHT 100

typedef uint16_t room_id_t;

typedef struct room_t {
    room_id_t id;
    uint16_t x, y;
    uint16_t width, height;
} room_t;

typedef struct editor_internal_data_t {
    room_t rooms[MAX_ROOMS];
    uint8_t room_slots[ROOM_SLOTS_SIZE];
} editor_internal_data_t;

static editor_internal_data_t editor_internal_data = {0};

bool is_room_slot_empty(room_id_t id) {
    if (id >= MAX_ROOMS) {
        return false;
    }

    if (id == ILLEGAL_ROOM_ID) {
        return false;
    }

    size_t index = id/8;
    uint8_t mask = 1 << (id%8);
    return (editor_internal_data.room_slots[index]&mask) == 0;
}

bool set_room_slot(room_id_t id, bool value) {
    if (id >= MAX_ROOMS) {
        return false;
    }

    if (id == ILLEGAL_ROOM_ID) {
        return false;
    }

    size_t index = id/8;
    uint8_t mask = 1 << (id%8);
    editor_internal_data.room_slots[index] &= ~mask;
    editor_internal_data.room_slots[index] |= (value&1) << (id%8);

    return true;
}

room_id_t allocate_room(room_t room) {
    room_id_t id = 0;
    bool found_sth = false;

    for (; id < MAX_ROOMS && id != ILLEGAL_ROOM_ID; id++) {
        if (is_room_slot_empty(id)) {
            set_room_slot(id, true);
            editor_internal_data.rooms[id] = room;
            editor_internal_data.rooms[id].id = id;
            found_sth = true;
            break;
        }
    }

    if (!found_sth) {
        return ILLEGAL_ROOM_ID;
    }

    return id;
}

bool free_room(room_id_t id) {
    return set_room_slot(id, false);
}

room_t *get_room(room_id_t id) {
    if (id >= MAX_ROOMS) {
        return NULL;
    }

    if (id == ILLEGAL_ROOM_ID) {
        return NULL;
    }

    if (is_room_slot_empty(id)) {
        return NULL;
    }

    return &editor_internal_data.rooms[id];
}

void save_bunker(const char *path) {
    FILE *file = fopen(path, "wb");

    fwrite("\0CUCBF", 1, 6, file);
    fwrite("\0\0", 1, 2, file);

    fflush(file);

    fclose(file);
}

Vector2 camera_pos = { 0, 0 };
Camera2D camera = { .zoom = 1.0f };
float zoom_factor = 0.2f;

room_id_t create_room(void) {
    imgui_vec2_t vec2 = imgui_get_mouse_world_coordinate();

    room_t room = {
        .x = roundf(vec2.x),
        .y = roundf(vec2.y),
        .width = DEFAULT_ROOM_WIDTH,
        .height = DEFAULT_ROOM_HEIGHT,
    };

    return allocate_room(room);
}

void room_gizmos(void) {

}

void draw_room(room_id_t id) {
    room_t *room = get_room(id);
    DrawRectangle(room->x, room->y, room->width, room->height, RED);
}

void draw_rooms() {
    for (size_t i = 0; i < MAX_ROOMS; i++) {
        if (is_room_slot_empty(i)) {
            continue;
        }

        draw_room(i);
    }
}

void bunker_editor(void) {
    static imgui_string_t bunker_editor_context_menu[] = {
        imgui_String("create room"),
    };

    const size_t context_item_count = (sizeof(bunker_editor_context_menu)/sizeof(bunker_editor_context_menu[0]));

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

        size_t choosen_context = imgui_contex_menu(bunker_editor_context_menu, context_item_count);

        if (choosen_context == 0) {            
            create_room();
        }

        draw_rooms();
    imgui_end_camera_mode();
    EndMode2D();
}

typedef enum current_editing_mode_t {
    EDITING_MODE_BUNKER = 0,
} current_editing_mode_t;

int main(void) {
    InitWindow(0, 0, "CUC Editor");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetTargetFPS(60);

    current_editing_mode_t current_editing_mode = EDITING_MODE_BUNKER;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (current_editing_mode == EDITING_MODE_BUNKER) {
            bunker_editor();
        }

        EndDrawing();
    }

    CloseWindow();

    // save_bunker("test.cucbf");
    
    return 0;
}