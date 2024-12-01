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
#define MAX_ROOMS 50
#define MAX_ENTITY_REFERENCES 50
#define MAX_ENVIRONMENT_ENTITIES 15
#define MAX_CLUE_COUNT 30
#define CLUE_METADATA_SIZE 4
#define ROOM_SLOTS_SIZE ((MAX_ROOMS+7)/8)
#define MAX_ROOM_CONNECTIONS 6
#define MAX_DRAW_CALLS 50

#define ILLEGAL_ENTITY_ID ((entity_id_t)-1)
#define ILLEGAL_ROOM_ID ((room_index_t)-1)

typedef uint16_t entity_id_t;
typedef uint16_t room_id_t;
typedef uint16_t room_index_t;

typedef void(*entity_handler_t)(entity_id_t entity_id);
typedef uint16_t entity_handler_id_t;

typedef struct entity_pos_t {
    room_index_t room_index;
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
    platform_camera_t camera;
} player_t;

typedef struct environment_entity_t {
    platform_texture_t texture;
    rectf_t dest;
} environment_entity_t;

typedef struct clue_pos_t {
    float x;
    float y;
} clue_pos_t;

typedef struct clue_t {
    uint16_t clue_type;
    clue_pos_t pos;
    float intensity;
    uint8_t metadata[CLUE_METADATA_SIZE];
} clue_t;

typedef struct rect_draw_call_t {
    rectf_t rect;
    color_t color;
} rect_draw_call_t;

typedef uint8_t draw_call_kind_t;
enum {
    DRAW_CALL_KIND_RECT = 0,
};

typedef union draw_call_as_t {
    rect_draw_call_t rect;
} draw_call_as_t;

typedef struct draw_call_t {
    draw_call_kind_t kind;
    draw_call_as_t as;
} draw_call_t;

typedef struct room_pos_t {
    uint32_t x;
    uint32_t y;
    uint16_t width;
    uint16_t height;
} room_pos_t;

typedef struct room_t {
    room_id_t id; 
    room_index_t index;
    room_pos_t pos;
    environment_entity_t environment_entities[MAX_ENVIRONMENT_ENTITIES];
    uint16_t environment_count;
    room_id_t connections[MAX_ROOM_CONNECTIONS];
    uint8_t connections_count;
    entity_id_t entities[MAX_ENTITY_REFERENCES];
    uint8_t entity_count;
    clue_t clues[MAX_CLUE_COUNT];
    uint8_t clue_count;
    draw_call_t draw_calls[MAX_DRAW_CALLS];
    uint8_t draw_call_count; 
} room_t;

typedef struct splitscreen_t {
    player_t player;
    platform_viewport_t viewport;
} splitscreen_t;

typedef struct cuc_engine_t {
    splitscreen_t splitscreens[MAX_SPLITSCREENS];
    uint8_t splitscreen_count;
    uint32_t screen_width;
    uint32_t screen_height;
    bool quit;

    entity_handler_t entity_handlers[MAX_ENTITY_HANDLERS];
    entity_t entities[MAX_ENTITIES];
    uint8_t entity_slots[ENTITY_SLOTS_SIZE];

    room_t rooms[MAX_ROOMS];
    uint8_t room_slots[ROOM_SLOTS_SIZE];
} cuc_engine_t;

// int x = sizeof(cuc_engine_t);
// int x = sizeof(room_t);

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
bool cuc_engine_unregister_entity(entity_id_t entity_id);
entity_t *cuc_engine_get_entity(entity_id_t entity_id);
bool cuc_engine_set_entity_handler(entity_handler_id_t handler_id, entity_handler_t handler);
entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id);
bool cuc_engine_is_entity_player(entity_id_t id, player_t **out_player);
bool cuc_engine_is_entity_splitscreen_player(entity_id_t id, splitscreen_t **out_splitscreen);

room_index_t cuc_engine_register_room(room_t room_index);
bool cuc_engine_unregister_room(room_index_t room_index);
room_t *cuc_engine_get_room(room_index_t room_index);
room_t *cuc_engine_get_room_using_id(room_id_t room_id);
room_id_t cuc_engine_room_index_to_id(room_index_t room_index);
room_index_t cuc_engine_room_id_to_index(room_id_t room_id);

bool cuc_engine_draw_rect(room_index_t room_index, rectf_t rect, color_t color);

#endif // _CUC_ENGINE_H_