#include "engine.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

static cuc_engine_t engine = {0};

void compute_splitscreen_rects(void);
void cuc_tick_update(void);
bool is_entity_slot_empty(entity_index_t entity_index);
bool set_entity_slot(entity_index_t entity_index, bool value);
bool is_room_slot_empty(room_index_t index);
bool set_room_slot(room_index_t index, bool value);
bool room_push_draw_call(room_index_t index, draw_call_t draw_call);
void draw_room(room_index_t index);
clue_stack_t *get_room_front_clue_stack(room_index_t room_index);
clue_stack_t *get_room_back_clue_stack(room_index_t room_index);
bool room_switch_clue_stacks(room_index_t room_index);

void cuc_engine_init(void) {
    platform_config_t config = {
        .width = 0, .height = 0,
        .title = "CUC",
        .target_fps = 60,
        .config_flags = PLATFORM_CONFIG_FLAG_BORDERLESS_WINDOWED | PLATFORM_CONFIG_FLAG_VSYNC*0,
    };

    platform_init(config);

    engine.screen_width = platform_get_window_size().x;
    engine.screen_height = platform_get_window_size().y;
}

void cuc_engine_deinit(void) {
    platform_deinit();
}

void cuc_engine_load_map(char *map_path) {
    printf("TODO: loading maps isn't implemented yet");
    assert(0 && "UNIMPLEMENTED");
}

bool cuc_engine_register_splitscreen(player_t player, uint8_t *out_id) {
    if (engine.splitscreen_count >= MAX_SPLITSCREENS) {
        return false;
    }

    splitscreen_t splitscreen = {0};
    splitscreen.player = player;
    splitscreen.player.camera.zoom = 1.0f;

    engine.splitscreens[engine.splitscreen_count] = splitscreen;

    if (out_id != NULL) {
        *out_id = engine.splitscreen_count;
    }

    engine.splitscreen_count += 1;

    compute_splitscreen_rects();

    return true;
}

bool cuc_engine_unregister_splitscreen(uint8_t id) {
    if (id >= engine.splitscreen_count) {
        return false;
    }

    engine.splitscreen_count -= 1;
    for (size_t i = id; i < engine.splitscreen_count && i < MAX_SPLITSCREENS; i++) {
        engine.splitscreens[i] = engine.splitscreens[i+1];
    }

    compute_splitscreen_rects();

    return true;
}

void cuc_engine_run(void) {
    while (!engine.quit) {
        cuc_engine_update();
    }
}

void cuc_engine_update(void) {
    if (!platform_running()) {
        engine.quit = true;
    }
    
    // float dt = platform_get_delta_time();
    if (platform_was_window_resized()) {
        engine.screen_width = platform_get_window_size().x;
        engine.screen_height = platform_get_window_size().y;
        compute_splitscreen_rects();
    }

    // NOTE: this is just temporary because i don't know how to clear the draw call stacks effeciently for not :>
    bool drawn_rooms[MAX_ROOMS] = {0};

    cuc_tick_update();

    platform_begin_drawing();
    for (size_t i = 0; i < engine.splitscreen_count; i++) {
        splitscreen_t *splitscreen = &engine.splitscreens[i];
        entity_t *player_entity = &engine.entities[splitscreen->player.id];
        // uint32_t width = splitscreen->viewport.width;
        // uint32_t height = splitscreen->viewport.height;

        platform_begin_viewport(splitscreen->viewport);
        platform_begin_camera(splitscreen->player.camera);
            platform_clear_background(COLOR_BLACK);
            // platform_clear_background(COLOR_RED);
            draw_room(player_entity->pos.room_index);
            
            drawn_rooms[player_entity->pos.room_index] = true;
        platform_end_camera();
        platform_end_viewport();
    }
        platform_draw_fps((vec2f_t) { 0, 0 });
    platform_end_drawing();

    for (size_t i = 0; i < MAX_ROOMS; i++) {
        if (!drawn_rooms[i]) {
            continue;
        }

        for (size_t j = 0; j < MAX_DRAW_LAYERS; j++) {
            engine.rooms[i].draw_layers[j] = (draw_layer_t) {0};
        }
    }
}

void cuc_engine_quit(void) {
    engine.quit = true;
}

float cuc_engine_get_tick_delta(void) {
    return platform_get_delta_time();
}

double cuc_engine_get_time(void) {
    return platform_get_time();
}

entity_index_t cuc_engine_register_entity(entity_t entity) {
    entity_index_t index = 0;
    bool found_sth = false;

    for (; index < MAX_ENTITIES && index != ILLEGAL_ENTITY_ID; index++) {
        if (is_entity_slot_empty(index)) {
            set_entity_slot(index, true);
            engine.entities[index] = entity;
            engine.entities[index].index = index;
            found_sth = true;
            break;
        }
    }

    if (!found_sth) {
        return ILLEGAL_ENTITY_ID;
    }

    return index;
}

bool cuc_engine_unregister_entity(entity_index_t entity_index) {
    return set_entity_slot(entity_index, false);
}

entity_t *cuc_engine_get_entity(entity_index_t entity_index) {
    if (entity_index >= MAX_ENTITIES || entity_index == ILLEGAL_ENTITY_ID || is_entity_slot_empty(entity_index)) {
        return NULL;
    }
    
    return &engine.entities[entity_index];
}

bool cuc_engine_set_entity_handler(entity_handler_id_t handler_id, entity_handler_t handler) {
    if (handler_id >= MAX_ENTITY_HANDLERS) {
        return false;
    }

    if (handler_id == EMPTY_ENTITY_HANDLER) {
        return false;
    }

    engine.entity_handlers[handler_id] = handler;

    return true;
}

entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id) {
    if (handler_id >= MAX_ENTITY_HANDLERS) {
        return NULL;
    }

    if (handler_id == EMPTY_ENTITY_HANDLER) {
        return NULL;
    }

    return engine.entity_handlers[handler_id];
}

bool cuc_engine_is_entity_player(entity_index_t id, player_t **out_player) {
    for (size_t i = 0; i < engine.splitscreen_count && i < MAX_SPLITSCREENS; i++) {
        if (engine.splitscreens[i].player.id == id) {
            if (out_player != NULL) {
                *out_player = &engine.splitscreens[i].player;
            }

            return true;
        }
    }

    return false;
}

bool cuc_engine_is_entity_splitscreen_player(entity_index_t id, splitscreen_t **out_splitscreen) {
        for (size_t i = 0; i < engine.splitscreen_count && i < MAX_SPLITSCREENS; i++) {
        if (engine.splitscreens[i].player.id == id) {
            if (out_splitscreen != NULL) {
                *out_splitscreen = &engine.splitscreens[i];
            }

            return true;
        }
    }

    return false;
}

entity_id_t cuc_engine_entity_index_to_id(entity_index_t entity_index) {
    if (entity_index >= MAX_ENTITIES) {
        return ILLEGAL_ENTITY_ID;
    }

    if (entity_index == ILLEGAL_ENTITY_ID) {
        return ILLEGAL_ENTITY_ID;
    }

    if (is_entity_slot_empty(entity_index)) {
        return ILLEGAL_ENTITY_ID;
    }

    return engine.entities[entity_index].id;
}

entity_index_t cuc_engine_entity_id_to_index(entity_id_t entity_id) {
    if (entity_id == ILLEGAL_ENTITY_ID) {
        return ILLEGAL_ENTITY_ID;
    }

    entity_index_t index = 0;

    for (; index < MAX_ENTITIES; index++) {
        if (is_entity_slot_empty(index)) {
            continue;
        }

        if (engine.entities[index].id == entity_id) {
            return index;
        }        
    }

    return ILLEGAL_ENTITY_ID;
}

room_index_t cuc_engine_register_room(room_t room) {
    room_index_t index = 0;
    bool found_sth = false;

    for (; index < MAX_ROOMS && index != ILLEGAL_ROOM_ID; index++) {
        if (is_room_slot_empty(index)) {
            set_room_slot(index, true);
            engine.rooms[index] = room;
            engine.rooms[index].index = index;
            engine.rooms[index].clue_buffer.stack_0.room_index = index;
            engine.rooms[index].clue_buffer.stack_1.room_index = index;
            found_sth = true;
            break;
        }
    }

    if (!found_sth) {
        return ILLEGAL_ROOM_ID;
    }

    return index;
}

bool cuc_engine_unregister_room(room_index_t room_index) {
    return set_room_slot(room_index, false);
}

room_t *cuc_engine_get_room(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return NULL;
    }

    if (room_index == ILLEGAL_ROOM_ID) {
        return NULL;
    }

    if (is_room_slot_empty(room_index)) {
        return NULL;
    }

    return &engine.rooms[room_index];
}

room_t *cuc_engine_get_room_using_id(room_id_t room_id) {
    return cuc_engine_get_room(cuc_engine_room_id_to_index(room_id));
}

room_id_t cuc_engine_room_index_to_id(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return ILLEGAL_ROOM_ID;
    }

    if (room_index == ILLEGAL_ROOM_ID) {
        return ILLEGAL_ROOM_ID;
    }

    if (is_room_slot_empty(room_index)) {
        return ILLEGAL_ROOM_ID;
    }

    return engine.rooms[room_index].id;
}

room_index_t cuc_engine_room_id_to_index(room_id_t room_id) {
    if (room_id == ILLEGAL_ROOM_ID) {
        return ILLEGAL_ROOM_ID;
    }
    
    room_index_t index = 0;
    
    for (; index < MAX_ROOMS; index++) {
        if (is_room_slot_empty(index)) {
            continue;
        }

        if (engine.rooms[index].id == room_id) {
            return index;
        }
    }

    return ILLEGAL_ROOM_ID;
}

bool cuc_engine_emit_clue(room_index_t room_index, clue_t clue) {
    clue_stack_t *back_clue_stack = get_room_back_clue_stack(room_index);

    if (back_clue_stack == NULL) {
        return false;
    }

    back_clue_stack->clues[back_clue_stack->clue_count] = clue;
    back_clue_stack->clue_count += 1;

    return true;
}

clue_query_t cuc_engine_query_clues(room_index_t room_index) {
    clue_query_t query = {0};

    room_t *room = cuc_engine_get_room(room_index);

    query.clue_stack_count = room->connections_count+1;
    query.clue_stacks[0] = get_room_front_clue_stack(room_index);

    for (size_t i = 1; i < query.clue_stack_count; i++) {
        query.clue_stacks[i] = get_room_front_clue_stack(room->connections[i-1].index);
    }

    return query;
}

bool cuc_engine_set_current_draw_layer(draw_layer_id_t layer) {
    if (layer >= MAX_DRAW_LAYERS) {
        return false;
    }

    engine.current_layer = layer;

    return true;
}

bool cuc_engine_draw_rect(room_index_t room_index, rectf_t rect, color_t color) {
    draw_call_t draw_call = {
        .kind = DRAW_CALL_KIND_RECT,
        .as = { .rect = { .rect = rect, .color = color } },
    };

    return room_push_draw_call(room_index, draw_call);
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// INTERNAL /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

void compute_splitscreen_rects(void) {
    int width = engine.screen_width;
    int height = engine.screen_height;

    switch (engine.splitscreen_count) {
    case 1: {
        width = engine.screen_width;
        height = engine.screen_height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;
    } break;
    case 2: {
        width = width/2;
        height = engine.screen_height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;
    } break;
    case 3: {
        width = width/3;
        height = height;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;

        engine.splitscreens[2].viewport.x = width*2;
        engine.splitscreens[2].viewport.y = 0;
        engine.splitscreens[2].viewport.width = width;
        engine.splitscreens[2].viewport.height = height;
    } break;
    case 4: {
        width = width/2;
        height = height/2;

        engine.splitscreens[0].viewport.x = 0;
        engine.splitscreens[0].viewport.y = 0;
        engine.splitscreens[0].viewport.width = width;
        engine.splitscreens[0].viewport.height = height;

        engine.splitscreens[1].viewport.x = width;
        engine.splitscreens[1].viewport.y = 0;
        engine.splitscreens[1].viewport.width = width;
        engine.splitscreens[1].viewport.height = height;

        engine.splitscreens[2].viewport.x = 0;
        engine.splitscreens[2].viewport.y = height;
        engine.splitscreens[2].viewport.width = width;
        engine.splitscreens[2].viewport.height = height;

        engine.splitscreens[3].viewport.x = width;
        engine.splitscreens[3].viewport.y = height;
        engine.splitscreens[3].viewport.width = width;
        engine.splitscreens[3].viewport.height = height;
    } break;
    }
}

void cuc_tick_update(void) {
    for (size_t i = 0; i < MAX_ROOMS && i != ILLEGAL_ROOM_ID; i++) {
        if (is_room_slot_empty(i)) {
            continue;
        }

        room_t *room = cuc_engine_get_room(i);

        if (room == NULL) {
            continue;
        }

        for (size_t j = 0; j < room->entity_count && j < MAX_ENTITY_REFERENCES; j++) {
            entity_t *entity = cuc_engine_get_entity(room->entities[j]);

            if (entity == NULL) {
                continue;;
            }

            entity_handler_t handler = cuc_engine_get_entity_handler(entity->handler_id);

            if (handler != NULL) {
                handler(entity->index);
            }
        }

        room_switch_clue_stacks(i);
    }
}

bool is_entity_slot_empty(entity_index_t entity_index) {
    if (entity_index >= MAX_ENTITIES) {
        return false;
    }

    if (entity_index == ILLEGAL_ENTITY_ID) {
        return false;
    }

    size_t index = entity_index/8;
    uint8_t mask = 1 << (entity_index%8);
    return (engine.entity_slots[index]&mask) == 0;
}

bool set_entity_slot(entity_index_t entity_index, bool value) {
    if (entity_index >= MAX_ENTITIES) {
        return false;
    }

    if (entity_index == ILLEGAL_ENTITY_ID) {
        return false;
    }

    size_t index = entity_index/8;
    uint8_t mask = 1 << (entity_index%8);
    engine.entity_slots[index] &= ~mask;
    engine.entity_slots[index] |= (value&1) << (entity_index%8);

    return true;
}

bool is_room_slot_empty(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return false;
    }

    if (room_index == ILLEGAL_ROOM_ID) {
        return false;
    }

    size_t index = room_index/8;
    uint8_t mask = 1 << (room_index%8);
    return (engine.room_slots[index]&mask) == 0; 
}

bool set_room_slot(room_index_t room_index, bool value) {
    if (room_index >= MAX_ROOMS) {
        return false;
    }

    if (room_index == ILLEGAL_ROOM_ID) {
        return false;
    }

    size_t index = room_index/8;
    uint8_t mask = 1 << (room_index%8);
    engine.room_slots[index] &= ~mask;
    engine.room_slots[index] |= (value&1) << (room_index%8);

    return true;
}

bool room_push_draw_call(room_index_t index, draw_call_t draw_call) {
    room_t *room = cuc_engine_get_room(index);

    draw_layer_id_t layer = engine.current_layer;
    draw_layer_t *draw_layer = &room->draw_layers[layer];

    if (draw_layer->draw_call_count >= MAX_DRAW_CALLS) {
        return false;
    }

    draw_layer->draw_calls[draw_layer->draw_call_count] = draw_call;
    draw_layer->draw_call_count += 1;

    return true;
}

void draw_room(room_index_t index) {
    room_t *room = cuc_engine_get_room(index);
    
    for (size_t i = 0; i < MAX_DRAW_LAYERS; i++) {
        draw_layer_t draw_layer = room->draw_layers[i];
        for (size_t j = 0; j < draw_layer.draw_call_count && j < MAX_DRAW_CALLS; j++) {
            draw_call_t draw_call = draw_layer.draw_calls[j];

            switch (draw_call.kind) {
            case DRAW_CALL_KIND_RECT: {
                rect_draw_call_t rect_draw_call = draw_call.as.rect;
                
                platform_draw_rect(rect_draw_call.rect, (vec2f_t) { rect_draw_call.rect.width/2.0f, rect_draw_call.rect.height/2.0f }, 0.0f, rect_draw_call.color);
            }break;
            default: {
                assert(0 && "Unknown draw call");
            }break;
            }
        }
    }
}

clue_stack_t *get_room_front_clue_stack(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return NULL;
    }

    if (room_index == ILLEGAL_ENTITY_ID) {
        return NULL;
    }

    room_t *room = cuc_engine_get_room(room_index);

    if (room == NULL) {
        return NULL;
    }

    if (room->clue_buffer.is_zero_stack_front) {
        return &room->clue_buffer.stack_0;
    } else {
        return &room->clue_buffer.stack_1;
    }
    
    return NULL;
}

clue_stack_t *get_room_back_clue_stack(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return NULL;
    }

    if (room_index == ILLEGAL_ENTITY_ID) {
        return NULL;
    }

    room_t *room = cuc_engine_get_room(room_index);

    if (room == NULL) {
        return NULL;
    }

    if (!room->clue_buffer.is_zero_stack_front) {
        return &room->clue_buffer.stack_0;
    } else {
        return &room->clue_buffer.stack_1;
    }
    
    return NULL;
}

bool room_switch_clue_stacks(room_index_t room_index) {
    if (room_index >= MAX_ROOMS) {
        return false;
    }

    if (room_index == ILLEGAL_ENTITY_ID) {
        return false;
    }

    room_t *room = cuc_engine_get_room(room_index);

    if (room == NULL) {
        return false;
    }

    if (room->clue_buffer.is_zero_stack_front) {
        room->clue_buffer.stack_0.clue_count = 0;
        room->clue_buffer.is_zero_stack_front = false;
    } else {
        room->clue_buffer.stack_1.clue_count = 0;
        room->clue_buffer.is_zero_stack_front = true;
    }

    return true;
}