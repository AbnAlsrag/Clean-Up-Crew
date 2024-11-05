#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "utils.h"
#include "player.h"

#include <stdint.h>
#include <raylib/raylib.h>

#define MAX_VIEWPORTS 4

typedef struct viewport_t {
    player_t *player;
    Camera2D camera;
    rectangle_t rect;
    RenderTexture2D fb;
} viewport_t;

typedef struct renderer_t {
    viewport_t viewports[MAX_VIEWPORTS];
    size_t viewport_count;
} renderer_t;

void init_render_contex(void);
viewport_t create_viewport(player_t *player);
bool renderer_register_viewport(viewport_t viewport);
void renderer_update();
void deinit_render_contex(void);

#endif // _RENDERER_H_