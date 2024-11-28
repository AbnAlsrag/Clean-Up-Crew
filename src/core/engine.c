#include "engine.h"

#include <stdio.h>
#include <math.h>

static cuc_engine_t engine = {0};

void compute_splitscreen_rects(void);
void cuc_tick_update(void);
bool is_entity_slot_empty(entity_id_t entity_id);
bool set_entity_slot(entity_id_t entity_id, bool value);

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
    *(uint8_t*)(void*)0 = 5;
}

bool cuc_engine_register_splitscreen(player_t player, uint8_t *out_id) {
    if (engine.splitscreen_count >= MAX_SPLITSCREENS) {
        return false;
    }

    splitscreen_t splitscreen = {0};
    splitscreen.player = player;
    splitscreen.camera.zoom = 1.0f;

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
    
    float dt = platform_get_delta_time();
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
        uint32_t width = splitscreen->viewport.width;
        uint32_t height = splitscreen->viewport.height;

        vec2f_t pos = (vec2f_t) { player_entity->pos.x, player_entity->pos.y };
        float lerp_factor = 1.0f - exp(-5.0 * dt);
        splitscreen->camera.target = lerpfv2(splitscreen->camera.target, pos, lerp_factor);
        splitscreen->camera.offset = (vec2f_t) { width/2.0f, height/2.0f };

        platform_begin_viewport(splitscreen->viewport);
        platform_begin_camera(splitscreen->camera);
            platform_clear_background(COLOR_BLACK);
            if (i == 0) {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_RED);
            } else {
                platform_draw_rect((rectf_t) { 0, 0, width, height }, VECTOR2_ZERO, 0.0f, COLOR_WHITE);
            }

            platform_draw_rect((rectf_t) { player_entity->pos.x, player_entity->pos.y, 100, 100 }, VECTOR2_ZERO, 0.0f, COLOR_PURPLE);
        
        platform_end_camera();
        platform_end_viewport();
    }
        // DrawFPS(10, 10);
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

    for (; id < MAX_ENTITIES; id++) {
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

void cuc_engine_unregister_entity(entity_id_t entity_id) {
    set_entity_slot(entity_id, false);
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

    engine.entity_handlers[handler_id] = handler;

    return true;
}

entity_handler_t cuc_engine_get_entity_handler(entity_handler_id_t handler_id) {
    if (handler_id >= MAX_ENTITY_HANDLERS) {
        return NULL;
    }

    return engine.entity_handlers[handler_id];
}

bool cuc_engine_register_smart_entity(entity_id_t id) {
    if (id >= MAX_SMART_ENTITIES) {
        return false;
    }

    engine.smart_entities[engine.smart_entities_count] = id;

    engine.smart_entities_count += 1;

    return true;
}

bool cuc_engine_register_room(room_t room, room_id_t *out_id) {
    if (engine.room_count >= MAX_ROOMS) {
        return false;
    }

    engine.rooms[engine.room_count] = room;

    if (out_id != NULL) {
        *out_id = engine.room_count;
    }

    engine.room_count += 1;

    return true;
}

bool cuc_engine_unregister_room(room_id_t room_id) {
    if (room_id >= engine.room_count) {
        return false;
    }

    engine.room_count -= 1;
    for (room_id_t id = room_id; id < engine.room_count && id < MAX_ROOMS; id++) {
        engine.rooms[id] = engine.rooms[id+1];
    }

    return true;
}

room_t *cuc_engine_get_room(room_id_t room_id) {
    if (room_id >= engine.room_count) {
        return NULL;
    }

    return &engine.rooms[room_id];
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
    for (size_t i = 0; i < engine.smart_entities_count; i++) {
        entity_id_t id = engine.smart_entities[i];
        cuc_engine_get_entity_handler(cuc_engine_get_entity(id)->handler_id)(id);
    }
}

bool is_entity_slot_empty(entity_id_t entity_id) {
    if (entity_id >= MAX_ENTITIES) {
        return false;
    }

    if (entity_id == ILLEGAL_ENTITY_ID) {
        return false;
    }

    uint32_t index = entity_id/8;
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

    uint32_t index = entity_id/8;
    uint8_t mask = 1 << (entity_id%8);
    engine.entity_slots[index] &= ~mask;
    engine.entity_slots[index] |= (value&1) << (entity_id%8);

    return true;
}