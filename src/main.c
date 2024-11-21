#include <stdio.h>
#include "core/renderer.h"
#include "platform/platform.h"
#include <raylib/raylib.h>
#include <raylib/rlgl.h>

int main(void) {
    init_render_contex();

    while (platform_running()) {
        renderer_update();
    }

    deinit_render_contex();
    return 0;
}