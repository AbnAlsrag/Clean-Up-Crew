#include <stdio.h>
#include "core/engine.h"
#include "platform/platform.h"

void empty_handler(entity_id_t id) {
    return;
}

void player_handler(entity_id_t id) {
    entity_t *player = cuc_engine_get_entity(id);

    float dt = cuc_engine_get_tick_delta();
    float speed = 400.0f;

    if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
        player->pos.y -= speed*dt;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
        player->pos.y += speed*dt;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
        player->pos.x -= speed*dt;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
        player->pos.x += speed*dt;
    }

    if (platform_is_key_pressed(PLATFORM_KEYCODE_F)) {
        cuc_engine_quit();
    }
}

int main(void) {
    cuc_engine_init();

    cuc_engine_set_entity_handler(0, empty_handler);
    cuc_engine_set_entity_handler(1, player_handler);

    entity_pos_t players_pos = { .room_id = 0, .x = 100, .y = 100 };

    entity_id_t player_id_0 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 1 });
    entity_id_t player_id_1 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 0 });
    entity_id_t player_id_2 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 0 });
    entity_id_t player_id_3 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 0 });

    cuc_engine_register_smart_entity(player_id_0);
    cuc_engine_register_smart_entity(player_id_1);
    cuc_engine_register_smart_entity(player_id_2);
    cuc_engine_register_smart_entity(player_id_3);

    cuc_engine_register_splitscreen((player_t) { .id = player_id_0 }, NULL);
    cuc_engine_register_splitscreen((player_t) { .id = player_id_1 }, NULL);
    cuc_engine_register_splitscreen((player_t) { .id = player_id_2 }, NULL);
    cuc_engine_register_splitscreen((player_t) { .id = player_id_3 }, NULL);

    cuc_engine_run();

    cuc_engine_deinit();
    return 0;
}