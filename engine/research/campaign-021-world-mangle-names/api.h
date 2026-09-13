#ifndef OP_WORLD_MANGLE_NAMES_API_H
#define OP_WORLD_MANGLE_NAMES_API_H
#include <stddef.h>
typedef struct OP_ACTOR
{
    unsigned char before_name[8];
    char name[32];
} OP_ACTOR;
typedef struct OP_WORLD
{
    unsigned char before_count[0x1e8];
    int count;
    unsigned char before_actors[8];
    OP_ACTOR **actors;
} OP_WORLD;
typedef char actor_name[offsetof(OP_ACTOR, name) == 8 ? 1 : -1];
typedef char actor_size[sizeof(OP_ACTOR) == 40 ? 1 : -1];
typedef char world_count[offsetof(OP_WORLD, count) == 0x1e8 ? 1 : -1];
typedef char world_actors[offsetof(OP_WORLD, actors) == 0x1f4 ? 1 : -1];
void op_world_mangle_names(OP_WORLD *world);
typedef char op_word_width[sizeof(int) == 4 && sizeof(unsigned int) == 4 && sizeof(void *) == 4 ? 1 : -1];
typedef char op_world_extent[sizeof(OP_WORLD) == 0x1f8 ? 1 : -1];
typedef char op_short_width[sizeof(unsigned short) == 2 ? 1 : -1];
#endif
