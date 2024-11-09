#ifndef _RESOURCE_MANGER_H_
#define _RESOURCE_MANGER_H_

#include "utils.h"

#include <raylib/raylib.h>

typedef struct resource_id_t {
    uint32_t id;
} resource_id_t;

typedef struct sprite_resource_t {
    Texture2D texture;
} sprite_resource_t;

typedef enum resource_kind_t {
    RESOURCE_KIND_SPRITE = 0,
    MAX_RESOURCE_KIND,
} resource_kind_t;

typedef union resource_as_t {
    sprite_resource_t sprite;
} resource_as_t;

typedef struct resource_t {
    resource_kind_t kind;
    resource_as_t as;
} resource_t;

typedef struct room_t

#endif // _RESOURCE_MANGER_H_