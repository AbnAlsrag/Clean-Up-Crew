#include <stdio.h>
#include "core/engine.h"
#include "platform/platform.h"

void player_handler(entity_id_t id) {
    entity_t *entity = cuc_engine_get_entity(id);
    splitscreen_t *splitscreen;

    if (!cuc_engine_is_entity_splitscreen_player(id, &splitscreen)) {
        return;
    }

    player_t *player = &splitscreen->player;

    float dt = cuc_engine_get_tick_delta();
    float speed = 400.0f;
    speed = speed*dt;
    vec2f_t velocity = VECTOR2_ZERO;

    if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
        velocity.y = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
        velocity.y = 1;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
        velocity.x = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
        velocity.x = 1;
    }

    // velocity = vec2f_normalize(velocity);
    velocity = vec2f_normalize(velocity);

    entity->pos.x += velocity.x*speed;
    entity->pos.y += velocity.y*speed;

    player->camera.target.x = entity->pos.x;
    player->camera.target.y = entity->pos.y;

    player->camera.offset = vec2f_div_value(platform_get_window_size(), 2.0f);

    cuc_engine_draw_rect(entity->pos.room_index, (rectf_t) { 0, 0, splitscreen->viewport.width, splitscreen->viewport.height }, COLOR_WHITE);

    cuc_engine_draw_rect(entity->pos.room_index, (rectf_t) { entity->pos.x, entity->pos.y, 100, 100 }, COLOR_RED);

    if (platform_is_key_pressed(PLATFORM_KEYCODE_F)) {
        cuc_engine_quit();
    }
}

int main(void) {
    cuc_engine_init();

    // cuc_engine_load_map("bunker.cucb");

    cuc_engine_set_entity_handler(1, player_handler);

    entity_pos_t players_pos = { .room_index = 0, .x = 0, .y = 0 };

    entity_id_t player_id_0 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 1 });

    cuc_engine_register_splitscreen((player_t) { .id = player_id_0 }, NULL);

    room_t room_0 = {0};
    room_0.entity_count = 1;
    room_0.entities[0] = player_id_0;
    room_0.id = 0;
    room_0.pos = (room_pos_t) { .x = 0, .y = 0, .width = 500, .height = 500 };

    cuc_engine_register_room(room_0);

    cuc_engine_run();

    cuc_engine_deinit();
    return 0;
}