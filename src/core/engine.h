#ifndef _CUC_ENGINE_H_
#define _CUC_ENGINE_H_

#include "utils.h"
#include "platform/platform.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_SPLITSCREENS 4
#define MAX_ENTITY_HANDLERS 100
#define MAX_ENTITIES 100
#define ENTITY_SLOTS_SIZE ((MAX_ENTITIES+7)/8)
#define MAX_ENTITY_REFERENCES 50
#define MAX_SMART_ENTITIES 20
#define MAX_ENVIRONMENT_ENTITIES 15
#define MAX_ROOMS 50
#define ILLEGAL_ENTITY_ID ((entity_id_t)-1)

typedef uint16_t entity_id_t;
typedef uint32_t room_id_t;

typedef void(*entity_handler_t)(entity_id_t entity_id);
typedef uint16_t entity_handler_id_t;

typedef struct entity_pos_t {
    room_id_t room_id;
    float x;
    float y;
} entity_pos_t;

typedef struct entity_t {
    entity_pos_t pos;
    entity_handler_id_t handler_id;
    entity_id_t id;
} entity_t;

typedef struct player_t {
    entity_id_t id;
} player_t;

typedef struct environment_entity_t {
    platform_texture_t texture;
    rectf_t dest;
} environment_entity_t;

typedef struct room_t {
    room_id_t id; 
    rectf_t rect;
    environment_entity_t environment_entities[MAX_ENVIRONMENT_ENTITIES];
    uint16_t environment_count;
} room_t;

typedef struct splitscreen_t {
    player_t player;
    platform_camera_t camera;
    platform_viewport_t viewport;
} splitscreen_t;

typedef struct cuc_engine_t {
    splitscreen_t splitscreens[MAX_SPLITSCREENS];
    uint8_t splitscreen_count;
    uint32_t screen_width;
    uint32_t screen_height;
    bool quit;

    entity_id_t smart_entities[MAX_SMART_ENTITIES];
    uint16_t smart_entities_count;

    entity_handler_t entity_handlers[MAX_ENTITY_HANDLERS];
    entity_t entities[MAX_ENTITIES];
    uint8_t entity_slots[ENTITY_SLOTS_SIZE];

    room_t rooms[MAX_ROOMS];
    uint16_t room_count;
} cuc_engine_t;

// int x = sizeof(cuc_engine_t);

void cuc_engine_init(void);
void cuc_engine_deinit(void);
void cuc_engine_load_map(char *map_path);
bool cuc_engine_register_splitscreen(player_t player, uint8_t *out_id);
bool cuc_engine_unregister_splitscreen(uint8_t id);
void cuc_engine_run(void);
void cuc_engine_update(void);
void cuc_engine_quit(void);
float cuc_engine_get_tick_delta(void);
entity_id_t cuc_engine_register_entity(entity_t entity);
void cuc_engine_unregister_entity(entity_id_t entity_id);
entity_t *cuc_engine_get_entity(entity_id_t entity_id);
bool cuc_engine_set_entity_handler(entity_handler_id_t handler_id, entity_handler_t handler);
entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id);
bool cuc_engine_register_smart_entity(entity_id_t id);
bool cuc_engine_register_room(room_t room, room_id_t *out_id);
bool cuc_engine_unregister_room(room_id_t room_id);
room_t *cuc_engine_get_room(room_id_t room_id);

#endif // _CUC_ENGINE_H_