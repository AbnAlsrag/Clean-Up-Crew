#include "engine.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

static cuc_engine_t engine = {0};

void compute_splitscreen_rects(void);
void cuc_tick_update(void);
bool is_entity_slot_empty(entity_id_t entity_id);
bool set_entity_slot(entity_id_t entity_id, bool value);
bool is_room_slot_empty(room_index_t index);
bool set_room_slot(room_index_t index, bool value);
bool room_push_draw_call(room_index_t index, draw_call_t draw_call);
void draw_room(room_index_t index);

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
        platform_end_camera();
        platform_end_viewport();
    }
        platform_draw_fps((vec2f_t) { 10, 10 });
    platform_end_drawing();
}

void cuc_engine_quit(void) {
    engine.quit = true;
}

float cuc_engine_get_tick_delta(void) {
    return platform_get_delta_time();
}

entity_id_t cuc_engine_register_entity(entity_t entity) {
    entity_id_t id = 0;
    bool found_sth = false;

    for (; id < MAX_ENTITIES && id != ILLEGAL_ENTITY_ID; id++) {
        if (is_entity_slot_empty(id)) {
            set_entity_slot(id, true);
            engine.entities[id] = entity;
            engine.entities[id].id = id;
            found_sth = true;
            break;
        }
    }

    if (!found_sth) {
        return ILLEGAL_ENTITY_ID;
    }

    return id;
}

bool cuc_engine_unregister_entity(entity_id_t entity_id) {
    return set_entity_slot(entity_id, false);
}

entity_t *cuc_engine_get_entity(entity_id_t entity_id) {
    if (entity_id >= MAX_ENTITIES || entity_id == ILLEGAL_ENTITY_ID || is_entity_slot_empty(entity_id)) {
        return NULL;
    }
    
    return &engine.entities[entity_id];
}

bool cuc_engine_set_entity_handler(entity_handler_id_t handler_id, entity_handler_t handler) {
    if (handler_id >= MAX_ENTITY_HANDLERS) {
        return false;
    }

    if (handler_id == 0) {
        return false;
    }

    engine.entity_handlers[handler_id] = handler;

    return true;
}

entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id) {
    if (handler_id >= MAX_ENTITY_HANDLERS) {
        return NULL;
    }

    if (handler_id == 0) {
        return NULL;
    }

    return engine.entity_handlers[handler_id];
}

bool cuc_engine_is_entity_player(entity_id_t id, player_t **out_player) {
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

bool cuc_engine_is_entity_splitscreen_player(entity_id_t id, splitscreen_t **out_splitscreen) {
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

room_index_t cuc_engine_register_room(room_t room) {
    room_index_t index = 0;
    bool found_sth = false;

    for (; index < MAX_ROOMS && index != ILLEGAL_ROOM_ID; index++) {
        if (is_room_slot_empty(index)) {
            set_room_slot(index, true);
            engine.rooms[index] = room;
            engine.rooms[index].index = index;
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
        if (engine.rooms[index].id == room_id) {
            return index;
        }
    }

    return ILLEGAL_ROOM_ID;
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
            break;
        }

        room_t *room = cuc_engine_get_room(i);

        if (room == NULL) {
            break;
        }

        for (size_t j = 0; j < room->entity_count && j < MAX_ENTITY_REFERENCES; j++) {
            entity_t *entity = cuc_engine_get_entity(room->entities[j]);

            if (entity == NULL) {
                break;
            }

            entity_handler_t handler = cuc_engine_get_entity_handler(entity->handler_id);

            if (handler != NULL) {
                handler(entity->id);
            }
        }
    }
}

bool is_entity_slot_empty(entity_id_t entity_id) {
    if (entity_id >= MAX_ENTITIES) {
        return false;
    }

    if (entity_id == ILLEGAL_ENTITY_ID) {
        return false;
    }

    size_t index = entity_id/8;
    uint8_t mask = 1 << (entity_id%8);
    return (engine.entity_slots[index]&mask) == 0;
}

bool set_entity_slot(entity_id_t entity_id, bool value) {
    if (entity_id >= MAX_ENTITIES) {
        return false;
    }

    if (entity_id == ILLEGAL_ENTITY_ID) {
        return false;
    }

    size_t index = entity_id/8;
    uint8_t mask = 1 << (entity_id%8);
    engine.entity_slots[index] &= ~mask;
    engine.entity_slots[index] |= (value&1) << (entity_id%8);

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

    if (room->draw_call_count >= MAX_DRAW_CALLS) {
        return false;
    }

    room->draw_calls[room->draw_call_count] = draw_call;
    room->draw_call_count += 1;

    return true;
}

void draw_room(room_index_t index) {
    room_t *room = cuc_engine_get_room(index);
    
    for (size_t i = 0; i < room->draw_call_count; i++) {
        draw_call_t draw_call = room->draw_calls[i];
        
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

    room->draw_call_count = 0;
}