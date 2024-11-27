#include <stdio.h>
#include "core/engine.h"
#include "platform/platform.h"

int main(void) {
    init_render_contex();

    vec2f_t player1 = { 100, 100 };
    vec2f_t player2 = { 100, 100 };
    vec2f_t player3 = { 100, 100 };
    // vec2f_t player4 = { 100, 100 };

    renderer_register_splitscreen(&player1, NULL);
    renderer_register_splitscreen(&player2, NULL);
    renderer_register_splitscreen(&player3, NULL);
    // renderer_register_splitscreen(&player4, NULL);

    float speed = 5.0f;

    while (platform_running()) {
        renderer_update();

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

    deinit_render_contex();
    return 0;
}