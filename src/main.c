#include "core/engine.h"
#include "platform/platform.h"

#include <stdio.h>
#include <math.h>

#define SMELL_CLUE 0

bool toggle_smell_clue = false;

void player_handler(entity_id_t id) {
    entity_t *entity = cuc_engine_get_entity(id);
    splitscreen_t *splitscreen;

    if (!cuc_engine_is_entity_splitscreen_player(id, &splitscreen)) {
        return;
    }

    player_t *player = &splitscreen->player;

    float dt = cuc_engine_get_tick_delta();
    float speed = 400.0f;
    speed = speed*dt;
    vec2f_t velocity = VECTOR2_ZERO;

    if (platform_is_key_down(PLATFORM_KEYCODE_W)) {
        velocity.y = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_S)) {
        velocity.y = 1;
    }

    if (platform_is_key_down(PLATFORM_KEYCODE_A)) {
        velocity.x = -1;
    } else if (platform_is_key_down(PLATFORM_KEYCODE_D)) {
        velocity.x = 1;
    }

    // velocity = vec2f_normalize(velocity);
    velocity = vec2f_normalize(velocity);

    entity->pos.x += velocity.x*speed;
    entity->pos.y += velocity.y*speed;

    player->camera.target.x = entity->pos.x;
    player->camera.target.y = entity->pos.y;

    player->camera.offset = vec2f_div_value((vec2f_t) { splitscreen->viewport.width, splitscreen->viewport.height }, 2.0f);

    cuc_engine_set_current_draw_layer(0);
    cuc_engine_draw_rect(entity->pos.room_index, (rectf_t) { 0, 0, splitscreen->viewport.width, splitscreen->viewport.height }, COLOR_WHITE);

    cuc_engine_set_current_draw_layer(1);
    cuc_engine_draw_rect(entity->pos.room_index, (rectf_t) { entity->pos.x, entity->pos.y, 100, 100 }, COLOR_RED);

    if (platform_is_key_pressed(PLATFORM_KEYCODE_F)) {
        toggle_smell_clue = !toggle_smell_clue;
        // cuc_engine_quit();
    }

    if (toggle_smell_clue) {
        clue_t clue = {
            .clue_type = SMELL_CLUE,
            .intensity = 1.0f,
            .pos = (clue_pos_t) { .x = entity->pos.x, .y = entity->pos.y },
        };

        cuc_engine_emit_clue(entity->pos.room_index, clue);
    }
}

void dog_handler(entity_id_t id) {
    entity_t *entity = cuc_engine_get_entity(id);

    if (entity == NULL) {
        return;
    }

    cuc_engine_set_current_draw_layer(1);
    cuc_engine_draw_rect(entity->pos.room_index, (rectf_t) { entity->pos.x, entity->pos.y, 100, 50 }, COLOR_BEIGE);

    clue_query_t query = cuc_engine_query_clues(entity->pos.room_index);

    for (size_t i = 0; i < query.clue_stack_count; i++) {
        clue_stack_t *clue_stack = query.clue_stacks[i];
        
        for (size_t j = 0; j < clue_stack->clue_count; j++) {
            clue_t clue = clue_stack->clues[j];
            
            if (clue.clue_type == SMELL_CLUE) {
                vec2f_t entity_pos = { entity->pos.x, entity->pos.y };

                entity_pos = vec2f_lerp(entity_pos, clue.pos, 0.01f);
                entity->pos.x = entity_pos.x;
                entity->pos.y = entity_pos.y;
            }
        }
    }
}

int main(void) {
    cuc_engine_init();

    // cuc_engine_load_map("bunker.cucb");

    cuc_engine_set_entity_handler(1, player_handler);
    cuc_engine_set_entity_handler(2, dog_handler);

    entity_pos_t players_pos = { .room_index = 0, .x = 0, .y = 0 };
    entity_pos_t dog_pos = { .room_index = 0, .x = 150, .y = 0 };

    entity_id_t player_id_0 = cuc_engine_register_entity((entity_t) { .pos = players_pos, .handler_id = 1 });
    entity_id_t dog_id_0 = cuc_engine_register_entity((entity_t) { .pos = dog_pos, .handler_id = 2 });

    cuc_engine_register_splitscreen((player_t) { .id = player_id_0 }, NULL);

    room_t room_0 = {0};
    room_0.entity_count = 2;
    room_0.entities[0] = player_id_0;
    room_0.entities[1] = dog_id_0;
    room_0.id = 0;
    room_0.pos = (room_pos_t) { .x = 0, .y = 0, .width = 500, .height = 500 };

    cuc_engine_register_room(room_0);

    cuc_engine_run();

    cuc_engine_deinit();
    return 0;
}