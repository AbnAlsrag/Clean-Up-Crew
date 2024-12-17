#include "core/engine.h"
#include "core/cuc_utils.h"
#include "platform/platform.h"

#include <stdio.h>
#include <math.h>

#define PLAYER_CLUE 0

#define PLAYERS_LAYER 0
#define PLAYER0_BULLETS_LAYER 2
#define PLAYER1_BULLETS_LAYER 3
#define BULLET_COUNT MAX_DRAW_CALLS

typedef struct bullet_t {
    physics_object_t obj;
    bool active;
} bullet_t;

texture_index_t player_texture = ILLEGAL_TEXTURE_ID;
float player_size = 100.0f;

physics_object_t player0 = { .pos = { 200, 300 }, .mass = 1.0f };
float player0_angle = 0.0f;
bullet_t player0_bullets[BULLET_COUNT] = {0};

physics_object_t player1 = { .pos = { 200, 500 }, .mass = 1.0f };
float player1_angle = 0.0f;
bullet_t player1_bullets[BULLET_COUNT] = {0};

void manger(entity_index_t manger_entity_index) {
    float dt = cuc_engine_get_tick_delta();

    if (platform_is_gamepad_button_down(0, PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        dt = dt/2;
    }
    
    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };

    float player_acceleration = 500.0f;
    float player_deceleration = 100.0f;
    float player_radius = player_size/2;

    rectf_t player0_rect = (rectf_t) { player0.pos.x, player0.pos.y, player_size, player_size };
    rectf_t player1_rect = (rectf_t) { player1.pos.x, player1.pos.y, player_size, player_size };
    vec2f_t player_center = (vec2f_t) { player_size/2.0f, player_size/2.0f };

    circle_t player0_circle = (circle_t) { .center = { player0.pos.x, player0.pos.y }, player_radius };
    circle_t player1_circle = (circle_t) { .center = { player1.pos.x, player1.pos.y }, player_radius };

    if (check_collision_circle_circle(player0_circle, player1_circle)) {
        physics_resolve_circle_collision(&player0, &player1, 1.0f);
    }

    vec2f_t player0_move_axis = (vec2f_t) { platform_get_gamepad_axis(0, PLATFORM_GAMEPAD_AXIS_LEFT_X), platform_get_gamepad_axis(0, PLATFORM_GAMEPAD_AXIS_LEFT_Y) };
    vec2f_t player1_move_axis = (vec2f_t) { platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_X), platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_Y) };

    if (fabsf(player0_move_axis.x) < 0.8f) {
        player0_move_axis.x = 0.0f;
    }

    if (fabsf(player0_move_axis.y) < 0.8f) {
        player0_move_axis.y = 0.0f;
    }

    if (fabsf(player1_move_axis.x) < 0.8f) {
        player1_move_axis.x = 0.0f;
    }

    if (fabsf(player1_move_axis.y) < 0.8f) {
        player1_move_axis.y = 0.0f;
    }

    player0_move_axis = vec2f_normalize(player0_move_axis);
    player1_move_axis = vec2f_normalize(player1_move_axis);

    if (vec2f_length(player0_move_axis) != 0.0f) {
        player0_angle = lerp_angle(player0_angle, vec2f_angle(player0_move_axis), 0.1f);
    }

    if (vec2f_length(player1_move_axis) != 0.0f) {
        player1_angle = lerp_angle(player1_angle, vec2f_angle(player1_move_axis), 0.1f);
    }

    if (vec2f_length(player0_move_axis) == 0.0f) {
        vec2f_t player0_deceleration_direction = vec2f_normalize(vec2f_opposite(player0.velocity));
        vec2f_t player0_deceleration_force = vec2f_mult_value(player0_deceleration_direction, player_deceleration);
        physics_apply_force(&player0, player0_deceleration_force);

        if (vec2f_length(player0.velocity) < 0.1f) {
            player0.velocity = VECTOR2_ZERO;
        }
    }

    if (vec2f_length(player1_move_axis) == 0.0f) {
        vec2f_t player1_deceleration_direction = vec2f_normalize(vec2f_opposite(player1.velocity));
        vec2f_t player1_deceleration_force = vec2f_mult_value(player1_deceleration_direction, player_deceleration);
        physics_apply_force(&player1, player1_deceleration_force);

        if (vec2f_length(player1.velocity) < 0.1f) {
            player1.velocity = VECTOR2_ZERO;
        }
    }

    vec2f_t player0_move_force = vec2f_mult_value(player0_move_axis, player_acceleration);
    vec2f_t player1_move_force = vec2f_mult_value(player1_move_axis, player_acceleration);

    physics_apply_force(&player0, player0_move_force);
    physics_apply_force(&player1, player1_move_force);

    physics_update_obj(&player0, dt);
    physics_update_obj(&player1, dt);

    player0.pos = vec2f_wrap_rect(player0.pos, window_rect);
    player1.pos = vec2f_wrap_rect(player1.pos, window_rect);

    cuc_engine_set_current_draw_layer(PLAYERS_LAYER);

    cuc_engine_draw_rect(0, player0_rect, player_center, player0_angle, COLOR_BLUE);
    cuc_engine_draw_rect(0, player1_rect, player_center, player1_angle, COLOR_RED);
}

int main(void) {
    cuc_engine_init();

    player_texture = cuc_engine_load_texture("resources/imgs/test.png", 0);

    cuc_engine_set_entity_handler(1, manger);
    // cuc_engine_set_entity_handler(3, player1_handler);

    entity_index_t manger_index = cuc_engine_register_entity((entity_t) { .pos = VECTOR2_ZERO, .room = 0, .handler_id = 1 });

    cuc_engine_register_splitscreen((player_t) { .index = manger_index }, NULL);

    room_t room_0 = {0};
    room_0.entity_count = 1;
    room_0.entities[0] = manger_index;
    room_0.id = 0;
    room_0.pos = (room_pos_t) { .x = 0, .y = 0, .width = 500, .height = 500 };

    cuc_engine_register_room(room_0);

    cuc_engine_run();

    cuc_engine_deinit();
    return 0;
}