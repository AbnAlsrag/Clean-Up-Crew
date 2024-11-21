#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "math_utils.h"

#include <stdint.h>

#define MAX_VIEWPORTS 4

// typedef struct viewport_t {
//     player_t *player;
//     Camera2D camera;
//     rectf_t rect;
//     RenderTexture2D fb;
// } viewport_t;

// typedef struct renderer_t {
//     viewport_t viewports[MAX_VIEWPORTS];
//     size_t viewport_count;
//     int screen_width;
//     int screen_height;
// } renderer_t;

void init_render_contex(void);
// viewport_t create_viewport(player_t *player);
// void compute_viewport_rects(void);
// bool renderer_register_viewport(viewport_t viewport, uint8_t *out_id);
// bool renderer_unregister_viewport(uint8_t id);
void renderer_update(void);
void deinit_render_contex(void);


#endif // _RENDERER_H_