#include "core/engine.h"
#include "core/cuc_utils.h"
#include "platform/platform.h"

#include <stdio.h>
#include <math.h>

#define PLAYER_CLUE 0

texture_index_t player_texture = ILLEGAL_TEXTURE_ID;
float player_size = 100.0f;

vec2f_t *player0_pos = NULL;
vec2f_t player0_velocity = { 0.0f, 0.0f };
// float player0_angle = 0.0f;

vec2f_t *player1_pos = NULL;
vec2f_t player1_velocity = { 0.0f, 0.0f };

void resolve_circle_collision(
    vec2f_t *pos1, vec2f_t *vel1, float r1, float m1,
    vec2f_t *pos2, vec2f_t *vel2, float r2, float m2) {

    // Vector between circle centers
    vec2f_t collision_vector = { pos2->x - pos1->x, pos2->y - pos1->y };

    // Distance between centers
    float distance = sqrtf(collision_vector.x * collision_vector.x + collision_vector.y * collision_vector.y);

    // Prevent division by zero
    if (distance == 0.0f) return;

    // Normalize collision vector to get the collision normal
    vec2f_t normal = { collision_vector.x / distance, collision_vector.y / distance };

    // Penetration depth
    float penetration = r1 + r2 - distance;

    // Separate the circles if overlapping
    if (penetration > 0.0f) {
        float total_mass = m1 + m2;

        // Push circle 1
        pos1->x -= normal.x * (penetration * m2 / total_mass);
        pos1->y -= normal.y * (penetration * m2 / total_mass);

        // Push circle 2
        pos2->x += normal.x * (penetration * m1 / total_mass);
        pos2->y += normal.y * (penetration * m1 / total_mass);
    }

    // Relative velocity
    vec2f_t relative_velocity = { vel2->x - vel1->x, vel2->y - vel1->y };

    // Velocity along the collision normal
    float velocity_along_normal = relative_velocity.x * normal.x +
                                   relative_velocity.y * normal.y;

    // If the circles are moving apart, do nothing
    if (velocity_along_normal > 0.0f) return;

    // Restitution coefficient for bounciness
    float restitution = 0.8f;

    // Compute the impulse scalar
    float j = -(1 + restitution) * velocity_along_normal;
    j /= (1 / m1 + 1 / m2);

    // Impulse vector
    vec2f_t impulse = { j * normal.x, j * normal.y };

    // Apply impulse to circle 1
    vel1->x -= impulse.x / m1;
    vel1->y -= impulse.y / m1;

    // Apply impulse to circle 2
    vel2->x += impulse.x / m2;
    vel2->y += impulse.y / m2;

    // Apply continuous pushing force to maintain motion
    if (penetration > 0.0f) {
        vel1->x -= normal.x * 0.1f / m1;  // Small constant force
        vel1->y -= normal.y * 0.1f / m1;

        vel2->x += normal.x * 0.1f / m2;
        vel2->y += normal.y * 0.1f / m2;
    }
}

void player0_handler(entity_index_t index) {
    entity_t *entity = cuc_engine_get_entity(index);

    if (player0_pos == NULL) {
        player0_pos = &entity->pos;
    }

    if (player1_pos == NULL) {
        return;
    }

    // printf("%p\n", (void*)player0_pos);

    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };

    float player_radius = player_size/2;
    rectf_t player0_rect = (rectf_t) { player0_pos->x, player0_pos->y, player_size, player_size };
    circle_t player0_circle = (circle_t) { .center = { player0_pos->x, player0_pos->y }, player_radius };
    circle_t player1_circle = (circle_t) { .center = { player1_pos->x, player1_pos->y }, player_radius };

    color_t player0_color = COLOR_BLACK;

    if (check_collision_circle_circle(player0_circle, player1_circle)) {
        player0_color = COLOR_RED;
        resolve_circle_collision(player0_pos, &player0_velocity, player_radius, 1,
        player1_pos, &player1_velocity, player_radius, 1);
    } else {
        player0_color = COLOR_BLUE;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
        player0_velocity.y = -5;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
        player0_velocity.y = 5;
    } else {
        player0_velocity.y = 0;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
        player0_velocity.x = -5;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
        player0_velocity.x = 5;
    } else {
        player0_velocity.x = 0;
    }

    *player0_pos = vec2f_add(*player0_pos, player0_velocity);

    *player0_pos = vec2f_wrap_rect(*player0_pos, window_rect);

    cuc_engine_draw_rect(
        entity->room, player0_rect,
        (vec2f_t) { player_size/2, player_size/2 }, 0.0f, player0_color);
}

void player1_handler(entity_index_t index) {
    entity_t *entity = cuc_engine_get_entity(index);

    if (player1_pos == NULL) {
        entity->pos = (vec2f_t) { 300, 300 };
    }

    player1_pos = &entity->pos;

    float window_offset = 100;
    rectf_t window_rect = {
            -window_offset, -window_offset,
            platform_get_window_size().x+window_offset*2, platform_get_window_size().y+window_offset*2 };

    *player1_pos = vec2f_add(*player1_pos, player1_velocity);

    *player1_pos = vec2f_wrap_rect(*player1_pos, window_rect);

    cuc_engine_draw_texture(
        entity->room, player_texture,
        cuc_engine_get_texture_src_rect(player_texture),
        (rectf_t) { player1_pos->x, player1_pos->y, player_size, player_size },
        (vec2f_t) { player_size/2, player_size/2 },
        0.0f);
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