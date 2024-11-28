#include <stdio.h>
#include "core/engine.h"
#include "platform/platform.h"

void player_handler(entity_id_t id) {
    float dt = cuc_engine_get_tick_delta();
    float speed = 400.0f;

    entity_t *player = cuc_engine_get_entity(id);

    if (player == NULL) {
        return;
    }

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
}

int main(void) {
    cuc_engine_init();

    cuc_engine_set_entity_handler(player_handler, 0);

    cuc_engine_load_map();

    while (platform_running()) {
        cuc_engine_update();
    }

    cuc_engine_deinit();
    return 0;
}