#ifndef _CUC_ENGINE_H_
#define _CUC_ENGINE_H_

#include "utils.h"
#include "platform/platform.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_SPLITSCREENS 4

typedef struct player_t {
    vec2f_t pos;
} player_t;

typedef struct splitscreen_t {
    player_t *player;
    platform_camera_t camera;
    platform_viewport_t viewport;
} splitscreen_t;

typedef struct cuc_engine_t {
    splitscreen_t splitscreens[MAX_SPLITSCREENS];
    size_t splitscreen_count;
    uint32_t screen_width;
    uint32_t screen_height;
} cuc_engine_t;

void cuc_engine_init(void);
void cuc_engine_deinit(void);
bool cuc_engine_register_splitscreen(player_t *player, uint8_t *out_id);
bool cuc_engine_unregister_splitscreen(uint8_t id);
void cuc_engine_update(void);

#endif // _CUC_ENGINE_H_