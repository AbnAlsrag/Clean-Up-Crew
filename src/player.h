#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "utils.h"

#include <stdint.h>

typedef struct player_t {
    vec2f_t position;
    size_t current_room;
} player_t;

player_t create_player(vec2f_t pos);

#endif // _PLAYER_H_