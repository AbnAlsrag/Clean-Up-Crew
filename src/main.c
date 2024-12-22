#include "core/engine.h"
#include "core/cuc_utils.h"
#include "platform/platform.h"

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <math.h>

#define PLAYER_CLUE 0

#define PLAYERS_LAYER 0
#define PLAYER0_BULLETS_LAYER 1
#define PLAYER1_BULLETS_LAYER 2
#define PLAYER_LIVES_LAYER 3
#define BULLET_COUNT MAX_DRAW_CALLS
#define ILLEGAL_BULLET ((size_t)-1)

typedef struct bullet_t {
    physics_object_t obj;
    bool active;
    bool hit;
} bullet_t;

font_index_t font = ILLEGAL_FONT_ID;

texture_index_t player_texture = ILLEGAL_TEXTURE_ID;
float player_size = 100.0f;

char health0_string[] = "health: 0";
char health1_string[] = "health: 0";
physics_object_t player0 = { .pos = { 200, 300 }, .mass = 5000.0f };
int player0_health = 5;
int player0_lives = 5;
float player0_angle = 0.0f;
bullet_t player0_bullets[BULLET_COUNT] = {0};

physics_object_t player1 = { .pos = { 200, 500 }, .mass = 1.0f };
int player1_health = 5;
int player1_lives = 5;
float player1_angle = 0.0f;
bullet_t player1_bullets[BULLET_COUNT] = {0};

uint8_t winner = 0;

size_t get_free_bullet(uint8_t player) {
    bullet_t *bullet_array = NULL;

    if (player == 0) {
        bullet_array = player0_bullets;
    } else if (player == 1) {
        bullet_array = player1_bullets;
    } else {
        return ILLEGAL_BULLET;
    }

    for (size_t i = 0; i < BULLET_COUNT; i++) {
        if (bullet_array[i].active == false) {
            return i;
        }
    }

    return ILLEGAL_BULLET;
}

void reset_state(void) {
    player0 = (physics_object_t) { .pos = { 200, 300 }, .mass = 1.0f };
    player1 = (physics_object_t) { .pos = { 200, 500 }, .mass = 1.0f };
    player0_health = 5;
    player1_health = 5;
    player0_angle = 0.0f;
    player1_angle = 0.0f;
    memset(player0_bullets, 0, sizeof(bullet_t)*BULLET_COUNT);
    memset(player1_bullets, 0, sizeof(bullet_t)*BULLET_COUNT);

    for (size_t i = 0; i < BULLET_COUNT; i++) {
        player0_bullets[i].obj.mass = 1.0f;
        player1_bullets[i].obj.mass = 1.0f;
    }

    winner = 0;
}

void update_game(void) {    
    float dt = cuc_engine_get_tick_delta();

    // if (platform_is_gamepad_button_down(0, PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
    //     dt = dt/2;
    // }
    
    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };

    float player_acceleration = 500.0f;
    float player_deceleration = 100.0f;
    float bullet_speed = 70000.0f;
    float player_radius = player_size/2;

    vec2f_t player_barrel_offset = (vec2f_t) { -player_size/2, 5 };
    vec2f_t player_center = (vec2f_t) { player_size/2.0f, player_size/2.0f };

    rectf_t player0_rect = (rectf_t) { player0.pos.x, player0.pos.y, player_size, player_size };
    rectf_t player0_barrel = { player0.pos.x, player0.pos.y, 30, 10 };

    rectf_t player1_rect = (rectf_t) { player1.pos.x, player1.pos.y, player_size, player_size };
    rectf_t player1_barrel = { player1.pos.x, player1.pos.y, 30, 10 };

    if (player0_health <= 0) {
        winner = 2;
        player0_lives -= 1;
    }

    if (player1_health <= 0) {
        winner = 1;
        player1_lives -= 1;
    }

    vec2f_t player0_move_axis = (vec2f_t) { platform_get_gamepad_axis(0, PLATFORM_GAMEPAD_AXIS_LEFT_X), platform_get_gamepad_axis(0, PLATFORM_GAMEPAD_AXIS_LEFT_Y) };
    vec2f_t player1_move_axis = (vec2f_t) { platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_X), platform_get_gamepad_axis(1, PLATFORM_GAMEPAD_AXIS_LEFT_Y) };

    // player0_move_axis.y = -player0_move_axis.y;
    // player1_move_axis.y = -player1_move_axis.y;
    // printf(VECTOR2_FMT"\n", VECTOR2_ARG(player0_move_axis));

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
        // player0_angle = vec2f_angle(player0_move_axis);
        player0_angle = lerp_angle(player0_angle, vec2f_angle(player0_move_axis), 0.1f);
    }

    if (vec2f_length(player1_move_axis) != 0.0f) {
        player1_angle = lerp_angle(player1_angle, vec2f_angle(player1_move_axis), 0.1f);
    }

    if (platform_is_gamepad_button_pressed(0, PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
        size_t bullet = get_free_bullet(0);
        
        if (bullet != ILLEGAL_BULLET) {
            player0_bullets[bullet].active = true;
            vec2f_t bullet_pos = vec2f_add((vec2f_t) { player0_barrel.x, player0_barrel.y }, vec2f_set_angle((vec2f_t) { -player_barrel_offset.x, player_barrel_offset.y-5 }, player0_angle));
            player0_bullets[bullet].obj.pos = bullet_pos;
            // player0_bullets[bullet].obj.pos = player0.pos;
            // player0_bullets[bullet].obj.velocity = player0.velocity;
            player0_bullets[bullet].obj.velocity = VECTOR2_ZERO;
            vec2f_t bullet_direction = vec2f_polar2cartesian((vec2f_t) { 1.0f, player0_angle });
            vec2f_t bullet_force = vec2f_mult_value(bullet_direction, bullet_speed);
            physics_apply_force(&player0_bullets[bullet].obj, bullet_force);
        }
    }

    if (platform_is_gamepad_button_pressed(1, PLATFORM_GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
        size_t bullet = get_free_bullet(1);
        
        if (bullet != ILLEGAL_BULLET) {
            player1_bullets[bullet].active = true;
            player1_bullets[bullet].obj.pos = player1.pos;
            // player0_bullets[bullet].obj.velocity = player0.velocity;
            player1_bullets[bullet].obj.velocity = VECTOR2_ZERO;
            vec2f_t bullet_direction = vec2f_polar2cartesian((vec2f_t) { 1.0f, player1_angle });
            vec2f_t bullet_force = vec2f_mult_value(bullet_direction, bullet_speed);
            physics_apply_force(&player1_bullets[bullet].obj, bullet_force);
        }
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

    cuc_engine_set_current_draw_layer(PLAYER_LIVES_LAYER);
    for (size_t i = 0; i < player0_lives; i++) {
        rectf_t life_rect = (rectf_t) { (20+5)*i, 0, 20, 20 };
        cuc_engine_draw_rect(0, life_rect, VECTOR2_ZERO, 0.0f, COLOR_BLUE);
    }

    for (size_t i = 0; i < player1_lives; i++) {
        rectf_t life_rect = (rectf_t) { (20+5)*i, 0, 20, 20 };
        life_rect.x += platform_get_window_size().x-((20+5)*5);
        cuc_engine_draw_rect(0, life_rect, VECTOR2_ZERO, 0.0f, COLOR_RED);
    }

    health0_string[8] = player0_health+'0';
    cuc_engine_draw_text(0, font, health0_string, (vec2f_t) { 5, 20 }, VECTOR2_ZERO, 0.0f, 46, 1.0f, COLOR_BLUE);
    health1_string[8] = player1_health+'0';
    cuc_engine_draw_text(0, font, health1_string, (vec2f_t) { platform_get_window_size().x-200, 20 }, VECTOR2_ZERO, 0.0f, 46, 1.0f, COLOR_RED);

    vec2f_t player0_move_force = vec2f_mult_value(player0_move_axis, player_acceleration);
    vec2f_t player1_move_force = vec2f_mult_value(player1_move_axis, player_acceleration);

    physics_apply_force(&player0, player0_move_force);
    physics_apply_force(&player1, player1_move_force);

    physics_update_obj(&player0, dt);
    physics_update_obj(&player1, dt);

    circle_t player0_circle = (circle_t) { .center = { player0.pos.x, player0.pos.y }, player_radius };
    circle_t player1_circle = (circle_t) { .center = { player1.pos.x, player1.pos.y }, player_radius };

    for (size_t i = 0; i < BULLET_COUNT; i++) {
        if (player0_bullets[i].active) {
            physics_update_obj(&player0_bullets[i].obj, dt);

            circle_t bullet_circle = { { player0_bullets[i].obj.pos.x, player0_bullets[i].obj.pos.y }, 10 };

            if (vec2f_out_of_rect(player0_bullets[i].obj.pos, window_rect)) {
                player0_bullets[i].active = false;
            }

            if (check_collision_circle_circle(player1_circle, bullet_circle) && !player0_bullets[i].hit) {
                player1_health -= 1;
                player0_bullets[i].hit = true;
                player0_bullets[i].active = false;
            } else {
                player0_bullets[i].hit = false;
            }

            cuc_engine_set_current_draw_layer(PLAYER0_BULLETS_LAYER);
            cuc_engine_draw_circle(0, bullet_circle, COLOR_MAGENTA);
        }

        if (player1_bullets[i].active) {
            physics_update_obj(&player1_bullets[i].obj, dt);

            circle_t bullet_circle = { { player1_bullets[i].obj.pos.x, player1_bullets[i].obj.pos.y }, 10 };

            if (vec2f_out_of_rect(player1_bullets[i].obj.pos, window_rect)) {
                player1_bullets[i].active = false;
            }

            if (check_collision_circle_circle(player0_circle, bullet_circle) && !player1_bullets[i].hit) {
                player0_health -= 1;
                player1_bullets[i].hit = true;
                player1_bullets[i].active = false;
            } else {
                player1_bullets[i].hit = false;
            }

            cuc_engine_set_current_draw_layer(PLAYER0_BULLETS_LAYER);
            cuc_engine_draw_circle(0, bullet_circle, COLOR_MAGENTA);
        }
    }

    if (check_collision_circle_circle(player0_circle, player1_circle)) {
        physics_resolve_circle_collision(&player0, &player1, 0.1f);
    }

    player0.pos = vec2f_wrap_rect(player0.pos, window_rect);
    player1.pos = vec2f_wrap_rect(player1.pos, window_rect);

    cuc_engine_set_current_draw_layer(PLAYERS_LAYER);

    cuc_engine_draw_rect(0, player0_rect, player_center, player0_angle, COLOR_BLUE);
    cuc_engine_draw_rect(0, player0_barrel, player_barrel_offset, player0_angle, COLOR_GREEN);

    cuc_engine_draw_rect(0, player1_rect, player_center, player1_angle, COLOR_RED);
    cuc_engine_draw_rect(0, player1_barrel, player_barrel_offset, player1_angle, COLOR_GREEN);
}

void manger(entity_index_t manger_entity_index) {
    if (winner == 0) {
        update_game();
    } else {
        if (player0_lives <= 0) {
            cuc_engine_set_clear_color(COLOR_WHITE);
            char *text = "RED WON";
            vec2f_t text_center = vec2f_div_value(platform_measure_text(cuc_engine_get_font(font)->font, text, 60, 2.0f), 2);
            vec2f_t screen_center = vec2f_div_value(platform_get_window_size(), 2);
            // cuc_engine_draw_text(0, font, "RED WON", VECTOR2_ZERO, VECTOR2_ZERO, 0.0f, 56, 2.0f, COLOR_RED);
            cuc_engine_draw_text(0, font, text, screen_center, text_center, 0.0f, 60, 2.0f, COLOR_RED);
        } else if (player1_lives <= 0) {
            cuc_engine_set_clear_color(COLOR_WHITE);
            char *text = "BLUE WON";
            vec2f_t text_center = vec2f_div_value(platform_measure_text(cuc_engine_get_font(font)->font, text, 60, 2.0f), 2);
            vec2f_t screen_center = vec2f_div_value(platform_get_window_size(), 2);
            cuc_engine_draw_text(0, font, text, screen_center, text_center, 0.0f, 60, 2.0f, COLOR_BLUE);
            // cuc_engine_set_clear_color(COLOR_BLUE);
        } else {
            reset_state();
        }
    }
}

int main(void) {
    for (size_t i = 0; i < BULLET_COUNT; i++) {
        player0_bullets[i].obj.mass = 1.0f;
        player1_bullets[i].obj.mass = 1.0f;
    }

    cuc_engine_init();

    font = cuc_engine_load_font_from_platform_font(platform_get_default_font(), 0);

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