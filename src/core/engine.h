#ifndef _CUC_ENGINE_H_
#define _CUC_ENGINE_H_

#include "cuc_utils.h"
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
#define MAX_DRAW_CALLS 15
#define MAX_DRAW_LAYERS 4
#define MAX_CLUE_QUERY_STACKS MAX_ROOM_CONNECTIONS+1
#define MAX_LOADED_TEXTURES 50
#define TEXTURE_SLOTS_SIZE ((MAX_LOADED_TEXTURES+7)/8)

#define ILLEGAL_ENTITY_ID ((entity_id_t)-1)
#define EMPTY_ENTITY_HANDLER ((entity_handler_id_t)0)
#define ILLEGAL_ROOM_ID ((room_id_t)-1)
#define ILLEGAL_TEXTURE_ID ((texture_id_t)-1)

typedef uint16_t entity_id_t;
typedef uint16_t entity_index_t;
typedef uint16_t room_id_t;
typedef uint16_t room_index_t;
typedef uint8_t draw_layer_id_t;
typedef uint16_t texture_id_t;
typedef uint16_t texture_index_t;

typedef void(*entity_handler_t)(entity_index_t entity_index);
typedef uint16_t entity_handler_id_t;

typedef struct entity_t {
    room_index_t room;
    vec2f_t pos;
    entity_handler_id_t handler_id;
    entity_id_t id;
    entity_index_t index;
} entity_t;

typedef struct player_t {
    entity_index_t id;
    platform_camera_t camera;
} player_t;

typedef struct environment_entity_t {
    platform_texture_t texture;
    rectf_t dest;
} environment_entity_t;

typedef vec2f_t clue_pos_t;

typedef struct clue_t {
    uint16_t clue_type;
    clue_pos_t pos;
    float intensity;
    uint8_t metadata[CLUE_METADATA_SIZE];
} clue_t;

typedef struct clue_stack_t {
    room_index_t room_index;
    clue_t clues[MAX_CLUE_COUNT];
    uint8_t clue_count;
} clue_stack_t;

typedef struct clue_buffer_t {
    clue_stack_t stack_0;
    clue_stack_t stack_1;
    bool is_zero_stack_front;
} clue_buffer_t;

typedef struct clue_query_t {
    clue_stack_t *clue_stacks[MAX_CLUE_QUERY_STACKS];
    size_t clue_stack_count;
} clue_query_t;

typedef struct rect_draw_call_t {
    rectf_t rect;
    vec2f_t origin;
    float rotation;
    color_t color;
} rect_draw_call_t;

typedef struct texture_draw_call_t {
    texture_index_t index;
    rectf_t src;
    rectf_t dest;
    vec2f_t origin;
    float rotation;
} texture_draw_call_t;

typedef uint8_t draw_call_kind_t;
enum {
    DRAW_CALL_KIND_RECT = 0,
    DRAW_CALL_KIND_TEXTURE,
};

typedef union draw_call_as_t {
    rect_draw_call_t rect;
    texture_draw_call_t texture;
} draw_call_as_t;

typedef struct draw_call_t {
    draw_call_kind_t kind;
    draw_call_as_t as;
} draw_call_t;

typedef struct draw_layer_t {
    draw_call_t draw_calls[MAX_DRAW_CALLS];
    uint16_t draw_call_count;
} draw_layer_t;

typedef struct room_pos_t {
    uint32_t x;
    uint32_t y;
    uint16_t width;
    uint16_t height;
} room_pos_t;

typedef struct room_connection_t {
    room_id_t id;
    room_index_t index;
} room_connection_t;

typedef struct room_t {
    room_id_t id; 
    room_index_t index;
    room_pos_t pos;
    environment_entity_t environment_entities[MAX_ENVIRONMENT_ENTITIES];
    uint16_t environment_count;
    room_connection_t connections[MAX_ROOM_CONNECTIONS];
    uint8_t connections_count;
    entity_index_t entities[MAX_ENTITY_REFERENCES];
    uint8_t entity_count;
    clue_buffer_t clue_buffer;
    draw_layer_t draw_layers[MAX_DRAW_LAYERS];
} room_t;

typedef struct splitscreen_t {
    player_t player;
    platform_viewport_t viewport;
} splitscreen_t;

typedef struct loaded_texture_t {
    texture_id_t id;
    platform_texture_t texture;
} loaded_texture_t;

typedef struct cuc_engine_t {
    splitscreen_t splitscreens[MAX_SPLITSCREENS];
    uint8_t splitscreen_count;
    uint32_t screen_width;
    uint32_t screen_height;
    bool quit;
    draw_layer_id_t current_layer;

    entity_handler_t entity_handlers[MAX_ENTITY_HANDLERS];
    entity_t entities[MAX_ENTITIES];
    uint8_t entity_slots[ENTITY_SLOTS_SIZE];

    room_t rooms[MAX_ROOMS];
    uint8_t room_slots[ROOM_SLOTS_SIZE];

    loaded_texture_t textures[MAX_LOADED_TEXTURES];
    uint8_t texture_slots[TEXTURE_SLOTS_SIZE];

    color_t clear_color;
} cuc_engine_t;

// int x = sizeof(cuc_engine_t); 
// int x = sizeof(room_t);
// int x = sizeof(draw_call_t);

void cuc_engine_init(void);
void cuc_engine_deinit(void);
void cuc_engine_load_map(char *map_path);
bool cuc_engine_register_splitscreen(player_t player, uint8_t *out_id);
bool cuc_engine_unregister_splitscreen(uint8_t id);
void cuc_engine_run(void);
void cuc_engine_update(void);
void cuc_engine_quit(void);
float cuc_engine_get_tick_delta(void);
double cuc_engine_get_time(void);

entity_index_t cuc_engine_register_entity(entity_t entity);
bool cuc_engine_unregister_entity(entity_index_t entity_index);
entity_t *cuc_engine_get_entity(entity_index_t entity_index);
bool cuc_engine_set_entity_handler(entity_handler_id_t handler_id, entity_handler_t handler);
entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id);
bool cuc_engine_is_entity_player(entity_index_t entity_index, player_t **out_player);
bool cuc_engine_is_entity_splitscreen_player(entity_index_t entity_index, splitscreen_t **out_splitscreen);
entity_id_t cuc_engine_entity_index_to_id(entity_index_t entity_index);
entity_index_t cuc_engine_entity_id_to_index(entity_id_t entity_id);

room_index_t cuc_engine_register_room(room_t room_index);
bool cuc_engine_unregister_room(room_index_t room_index);
room_t *cuc_engine_get_room(room_index_t room_index);
room_t *cuc_engine_get_room_using_id(room_id_t room_id);
room_id_t cuc_engine_room_index_to_id(room_index_t room_index);
room_index_t cuc_engine_room_id_to_index(room_id_t room_id);

texture_index_t cuc_engine_load_texture(const char *path, texture_id_t id);
bool cuc_engine_unload_texture(texture_index_t index);
loaded_texture_t *cuc_engine_get_texture(texture_index_t index);
rectf_t cuc_engine_get_texture_src_rect(texture_index_t index);

bool cuc_engine_emit_clue(room_index_t room_index, clue_t clue);
clue_query_t cuc_engine_query_clues(room_index_t room_index);

void cuc_engine_set_clear_color(color_t color);
bool cuc_engine_set_current_draw_layer(draw_layer_id_t layer);
bool cuc_engine_draw_rect(room_index_t room_index, rectf_t rect, vec2f_t origin, float rotation, color_t color);
bool cuc_engine_draw_texture(room_index_t room_index, texture_index_t texture_index, rectf_t src, rectf_t dest, vec2f_t origin, float rotation);

#endif // _CUC_ENGINE_H_