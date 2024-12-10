#ifndef _CUC_BFS_H_
#define _CUC_BFS_H_

#include <stdint.h>
#include <stdbool.h>

#define CUC_BF_MAGIC "\0CUCBF"
#define CUC_BF_MAGIC_LEN (sizeof(CUC_BF_MAGIC)-1)

#define CUC_BF_HEADER_DATA_SIZE 8
#define CUC_BF_ROOM_DATA_SIZE 10
#define CUC_BF_ILLEGAL_HEADER ((cuc_bf_header_t){0})
#define CUC_BF_ILLEGAL_ROOM_ID ((uint16_t)-1)
#define CUC_BF_ILLEGAL_ROOM ((cuc_bf_room_t) { .id = CUC_BF_ILLEGAL_ROOM_ID, .x = 0, .y = 0, .width = 0, .height = 0 })

#define CUC_LF_MAGIC "\0CUCLF"
#define CUC_LF_MAGIC_LEN (sizeof(CUC_LF_MAGIC)-1)

#define CUC_RF_MAGIC "\0CUCRF"
#define CUC_RF_MAGIC_LEN (sizeof(CUC_RF_MAGIC)-1)

typedef struct cuc_bf_header_t {
    uint8_t magic[CUC_BF_MAGIC_LEN];
    uint16_t room_count;
} cuc_bf_header_t;

typedef struct cuc_bf_room_t {
    uint16_t id;
    uint16_t x, y;
    uint16_t width, height;
} cuc_bf_room_t;

typedef struct cuc_bf_load_stream_t {
    cuc_bf_header_t header;
    void *stream;
} cuc_bf_load_stream_t;

cuc_bf_load_stream_t cuc_bfs_open_bf_stream(char *path);
void cuc_bfs_close_bf_stream(cuc_bf_load_stream_t *stream);
cuc_bf_room_t cuc_bfs_get_room_from_bf_stream(cuc_bf_load_stream_t stream, uint16_t room_id);

#define CUC_BFS_IMPLEMENTATION
#ifdef CUC_BFS_IMPLEMENTATION

#include <stdio.h>

cuc_bf_header_t cuc_bfs_bf_stream_read_bf_header(cuc_bf_load_stream_t stream) {
    cuc_bf_header_t result = CUC_BF_ILLEGAL_HEADER;

    if (stream.stream == NULL) {
        return CUC_BF_ILLEGAL_HEADER;
    }

    fread(&result.magic, 1, CUC_BF_MAGIC, stream.stream);
    fread(&result.room_count, sizeof(uint16_t), 1, stream.stream);

    return result;
}

cuc_bf_load_stream_t cuc_bfs_open_bf_stream(char *path) {
    cuc_bf_load_stream_t result;
    
    FILE *file = fopen(path, "rb");

    result.stream = file;

    if (result.stream == NULL) {
        result.header = CUC_BF_ILLEGAL_HEADER;
        return result;
    }

    result.header = cuc_bfs_bf_stream_read_bf_header(result);
   
    return result;
}

void cuc_bfs_close_bf_stream(cuc_bf_load_stream_t *stream) {
    if (stream->stream == NULL) {
        return;
    }
    
    fclose(stream->stream);

    stream->stream = NULL;
}

cuc_bf_room_t cuc_bfs_get_room_from_bf_stream(cuc_bf_load_stream_t stream, uint16_t room_id) {
    if (stream.stream == NULL) {
        return CUC_BF_ILLEGAL_ROOM;
    }

    if (room_id >= stream.header.room_count) {
        return CUC_BF_ILLEGAL_ROOM;
    }

    cuc_bf_room_t result = CUC_BF_ILLEGAL_ROOM;

    size_t offset = CUC_BF_HEADER_DATA_SIZE + (CUC_BF_ROOM_DATA_SIZE * room_id);
    fseek(SEEK_SET, offset, SEEK_SET);

    fread(&result, CUC_BF_ROOM_DATA_SIZE, 1, stream.stream);

    return result;
}

#endif // CUC_BFS_IMPLEMENTATION

#endif // _CUC_BFS_H_