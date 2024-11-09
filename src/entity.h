#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "utils.h"

#include <stdint.h>

typedef struct entity_pos_t {
    uint16_t room_id;
    float x, y;
} entity_pos_t;

typedef struct player_t {
    
} player_t;

typedef entity_t {
    entity_pos_t position;

} entity_t;

player_t create_player(entity_pos_t pos);

#endif // _ENTITY_H_