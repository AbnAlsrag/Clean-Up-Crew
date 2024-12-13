#include "core/engine.h"
#include "core/cuc_utils.h"
#include "platform/platform.h"

#include <stdio.h>
#include <math.h>

#define PLAYER_CLUE 0

texture_index_t player_texture = ILLEGAL_TEXTURE_ID;
float player_size = 100.0f;

physics_object_t player0 = { .pos = { 100, 400 }, .mass = 1.0f };
physics_object_t player1 = { .pos = { 200, 500 }, .mass = 1.0f };

void player0_handler(entity_index_t index) {
    entity_t *entity = cuc_engine_get_entity(index);

    float dt = cuc_engine_get_tick_delta();

    // // printf("%p\n", (void*)player0_pos);

    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };

    float player_speed = 500.0f;
    float friction_value = 0.0f;
    float player_radius = player_size/2;
    // rectf_t player0_rect = (rectf_t) { player0.pos.x, player0.pos.y, player_size, player_size };
    circle_t player0_circle = (circle_t) { .center = { player0.pos.x, player0.pos.y }, player_radius };
    circle_t player1_circle = (circle_t) { .center = { player1.pos.x, player1.pos.y }, player_radius };

    color_t player0_color = COLOR_BLACK;

    if (check_collision_circle_circle(player0_circle, player1_circle)) {
        player0_color = COLOR_RED;
        physics_resolve_circle_collision(&player0, &player1, 0.5f);
        // resolve_circle_collision(player0_pos, &player0_velocity, player_radius, 1,
        // player1_pos, &player1_velocity, player_radius, 1);
    } else {
        player0_color = COLOR_BLUE;
    }

    vec2f_t move_axis = VECTOR2_ZERO;

    if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
        move_axis.y = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
        move_axis.y = 1;
    } else {
        move_axis.y = 0;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
        move_axis.x = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
        move_axis.x = 1;
    } else {
        move_axis.x = 0;
    }

    move_axis = vec2f_normalize(move_axis);

    vec2f_t force = vec2f_mult_value(move_axis, player_speed);

    physics_apply_force(&player0, force);

    vec2f_t friction_direction = vec2f_opposite(vec2f_normalize(player0.velocity));

    // vec2f_t move_direction = vec2f_normalize(player0.velocity);
    // printf("move = "VECTOR2_FMT", ""friction = "VECTOR2_FMT"\n", VECTOR2_ARG(move_direction), VECTOR2_ARG(friction_direction));

    vec2f_t friction_force = vec2f_mult_value(friction_direction, friction_value);

    // friction_force = VECTOR2_ZERO;

    physics_apply_force(&player0, friction_force);

    physics_update_obj(&player0, dt);

    player0.pos = vec2f_wrap_rect(player0.pos, window_rect);

    entity->pos = player0.pos;

    // cuc_engine_draw_rect(
    //     entity->room, player0_rect,
    //     (vec2f_t) { player_size/2, player_size/2 }, 0.0f, player0_color);
    cuc_engine_draw_circle(entity->room, (circle_t) { .center = player0.pos, .radius = player_radius }, player0_color);
}

void player1_handler(entity_index_t index) {
    entity_t *entity = cuc_engine_get_entity(index);

    float dt = cuc_engine_get_tick_delta();

    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };;

    physics_update_obj(&player1, dt);

    player1.pos = vec2f_wrap_rect(player1.pos, window_rect);

    entity->pos = player1.pos;

    cuc_engine_draw_texture(
        entity->room, player_texture,
        cuc_engine_get_texture_src_rect(player_texture),
        (rectf_t) { player1.pos.x, player1.pos.y, player_size, player_size },
        (vec2f_t) { player_size/2.0f, player_size/2.0f },
        0.0f
    );
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