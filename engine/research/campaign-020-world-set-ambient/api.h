#ifndef OP_WORLD_SET_AMBIENT_API_H
#define OP_WORLD_SET_AMBIENT_API_H
#include <stddef.h>
typedef struct OP_WORLD
{
    unsigned char before_ambient[0x60];
    float ambient;
    unsigned char remainder[0xc78];
} OP_WORLD;
typedef char op_world_extent[sizeof(OP_WORLD) == 0xcdc ? 1 : -1];
typedef char op_world_words[sizeof(unsigned int) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef char op_world_ambient_offset[offsetof(OP_WORLD, ambient) == 0x60 ? 1 : -1];
void op_world_set_ambient(OP_WORLD *world, float ambient);
#endif