#include <stdio.h>
#include "core/engine.h"
#include "platform/platform.h"

int main(void) {
    cuc_engine_init();

    vec2f_t player1 = { 100, 100 };
    vec2f_t player2 = { 100, 100 };
    vec2f_t player3 = { 100, 100 };
    // vec2f_t player4 = { 100, 100 };

    cuc_engine_register_splitscreen((void*)&player1, NULL);
    cuc_engine_register_splitscreen((void*)&player2, NULL);
    cuc_engine_register_splitscreen((void*)&player3, NULL);
    // cuc_engine_register_splitscreen(&player4, NULL);

    float speed = 5.0f;

    while (platform_running()) {
        cuc_engine_update();

        if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
            player1.y -= speed;
        } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
            player1.y += speed;
        }

        if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
            player1.x -= speed;
        } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
            player1.x += speed;
        }

        if (platform_is_key_down(PLATFORM_KEYCODE_UP)) {
            player2.y -= speed;
        } else if (platform_is_key_down(PLATFORM_KEYCODE_DOWN)) {
            player2.y += speed;
        }

        if (platform_is_key_down(PLATFORM_KEYCODE_LEFT)) {
            player2.x -= speed;
        } else if (platform_is_key_down(PLATFORM_KEYCODE_RIGHT)) {
            player2.x += speed;
        }
    }

    cuc_engine_deinit();
    return 0;
}