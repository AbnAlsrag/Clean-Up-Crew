#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "utils.h"
#include "platform/platform.h"

#include <stdint.h>

#define MAX_SPLITSCREENS 4

typedef struct splitscreen_t {
    vec2f_t *player_position;
    platform_camera_t camera;
    platform_viewport_t viewport;
} splitscreen_t;

typedef struct renderer_t {
    splitscreen_t splitscreens[MAX_SPLITSCREENS];
    size_t splitscreen_count;
    int screen_width;
    int screen_height;
} renderer_t;

void init_render_contex(void);
void compute_splitscreen_rects(void);
bool renderer_register_splitscreen(vec2f_t *player_position, uint8_t *out_id);
bool renderer_unregister_splitscreen(uint8_t id);
void renderer_update(void);
void deinit_render_contex(void);


#endif // _RENDERER_H_