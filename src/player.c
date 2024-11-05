#include "player.h"

player_t create_player(vector2_t pos) {
    player_t player = { .position = pos, .current_room = 0 };

    return player;
}