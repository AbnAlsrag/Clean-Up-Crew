#include "entity.h"

player_t create_player(entity_pos_t pos) {
    player_t player = { .position = pos };

    return player;
}