#ifndef OP_WORLD_READ_ANIMATION_NAMES_API_H
#define OP_WORLD_READ_ANIMATION_NAMES_API_H
#include <stddef.h>
typedef struct OP_STREAM
{
    void *handle;
    int mode;
} OP_STREAM;
typedef union OP_NAME {
    int length;
    char *name;
} OP_NAME;
typedef struct OP_HEADER
{
    unsigned char before_count[0xa8];
    unsigned int count, payload_size;
} OP_HEADER;
typedef struct OP_WORLD
{
    unsigned char before_count[0x1e8];
    unsigned int count, payload_size;
    OP_NAME *names;
    void *actors;
} OP_WORLD;
typedef char name_width[sizeof(OP_NAME) == 4 ? 1 : -1];
typedef char header_count[offsetof(OP_HEADER, count) == 0xa8 ? 1 : -1];
typedef char header_payload[offsetof(OP_HEADER, payload_size) == 0xac ? 1 : -1];
typedef char world_count[offsetof(OP_WORLD, count) == 0x1e8 ? 1 : -1];
typedef char world_payload[offsetof(OP_WORLD, payload_size) == 0x1ec ? 1 : -1];
typedef char world_names[offsetof(OP_WORLD, names) == 0x1f0 ? 1 : -1];
typedef char world_actors[offsetof(OP_WORLD, actors) == 0x1f4 ? 1 : -1];
void *op_allocate(unsigned int size);
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream);
int op_world_read_animation_names(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header);
typedef char op_word_width[sizeof(int) == 4 && sizeof(unsigned int) == 4 && sizeof(void *) == 4 ? 1 : -1];
typedef char op_header_extent[sizeof(OP_HEADER) == 0xb0 ? 1 : -1];
typedef char op_world_extent[sizeof(OP_WORLD) == 0x1f8 ? 1 : -1];
typedef char op_stream_extent[sizeof(OP_STREAM) == 8 && offsetof(OP_STREAM, mode) == 4 ? 1 : -1];
typedef char op_short_width[sizeof(unsigned short) == 2 ? 1 : -1];
#endif
