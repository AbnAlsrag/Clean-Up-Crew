#include "core/engine.h"
#include "core/cuc_utils.h"
#include "platform/platform.h"

#include <stdio.h>
#include <math.h>

#define PLAYER_CLUE 0

texture_index_t player_texture = ILLEGAL_TEXTURE_ID;
vec2f_t velocity0 = { 0.0f, 0.0f };
vec2f_t velocity1 = { 0.0f, 0.0f };
float angle0 = 0.0f;
float angle1 = 0.0f;
vec2f_t last_move_axis0 = { 0.0f, 0.0f };
float time0 = 0.0f;

void player0_handler(entity_index_t index) {
    entity_t *entity = cuc_engine_get_entity(index);
    float dt = cuc_engine_get_tick_delta();

    float acceleration = 5.0f;
    float deceleration = 0.5f; // Add some deceleration
    vec2f_t move_axis = VECTOR2_ZERO;

    move_axis.x = platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_X);
    move_axis.y = platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_Y);

    // Threshold for detecting valid input
    const float dead_zone = 0.1f;

    if (fabs(move_axis.x) < dead_zone) move_axis.x = 0.0f;
    if (fabs(move_axis.y) < dead_zone) move_axis.y = 0.0f;

    // Normalize movement direction
    if (vec2f_length(move_axis) > 0.5f) {
        move_axis = vec2f_normalize(move_axis);
    } else {
        move_axis = VECTOR2_ZERO;
    }

    // Apply acceleration based on input
    velocity0.x += (acceleration * move_axis.x) * dt;
    velocity0.y += (acceleration * move_axis.y) * dt;

    // Apply deceleration when no input
    if (vec2f_equal(move_axis, VECTOR2_ZERO)) {
        velocity0.x *= (1.0f - deceleration * dt);
        velocity0.y *= (1.0f - deceleration * dt);
    }

    // Clamp to max speed (optional)
    const float max_speed = 200.0f;
    velocity0.x = fminf(fmaxf(velocity0.x, -max_speed), max_speed);
    velocity0.y = fminf(fmaxf(velocity0.y, -max_speed), max_speed);

    // Update position based on velocity
    entity->pos = vec2f_add(entity->pos, vec2f_mult_value(velocity0, dt));

    // Wrap position around the window
    rectf_t window_rect = (rectf_t) {
        .x = -100.0f,
        .y = -100.0f,
        .width = platform_get_window_size().x + 200.0f,
        .height = platform_get_window_size().y + 200.0f,
    };
    entity->pos = vec2f_wrap_rect(entity->pos, window_rect);

    // Draw the player
    vec2f_t texture_origin = (vec2f_t) { 100.0f / 2.0f, 100.0f / 2.0f };
    cuc_engine_draw_texture(entity->room, player_texture,
                            cuc_engine_get_texture_src_rect(player_texture),
                            (rectf_t) { entity->pos.x, entity->pos.y, 100, 100 },
                            texture_origin, angle0);
}

int main(void) {
    cuc_engine_init();

    player_texture = cuc_engine_load_texture("resources/imgs/test.png", 0);

    cuc_engine_set_entity_handler(1, player0_handler);
    cuc_engine_set_entity_handler(3, player1_handler);

    entity_id_t player_id_0 = cuc_engine_register_entity((entity_t) { .pos = VECTOR2_ZERO, .room = 0, .handler_id = 1 });
    entity_id_t player_id_1 = cuc_engine_register_entity((entity_t) { .pos = VECTOR2_ZERO, .room = 0, .handler_id = 3 });
    entity_id_t goose_id = cuc_engine_register_entity((entity_t) { .pos = (vec2f_t) { 100, 100 }, .room = 0, .handler_id = 2 });

    cuc_engine_register_splitscreen((player_t) { .id = player_id_0 }, NULL);

    room_t room_0 = {0};
    room_0.entity_count = 3;
    room_0.entities[0] = player_id_0;
    room_0.entities[1] = goose_id;
    room_0.entities[2] = player_id_1;
    room_0.id = 0;
    room_0.pos = (room_pos_t) { .x = 0, .y = 0, .width = 500, .height = 500 };

    cuc_engine_register_room(room_0);

    cuc_engine_run();

    cuc_engine_deinit();
    return 0;
}