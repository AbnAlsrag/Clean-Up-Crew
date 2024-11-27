#ifndef _ENTITIES_H_
#define _ENTITIES_H_

#include "utils.h"

typedef struct entity_t entity_t;

typedef (*entity_handler_t)(entity_t *entity);
typedef uint16_t entity_handler_id_t;

typedef struct entity_pos_t {
    uint16_t room_id;
    float x;
    float y;
} entity_pos_t;

struct entity_t {
    entity_pos_t pos;
    entity_handler_id_t handler_id;
};

#endif // _ENTITIES_H_