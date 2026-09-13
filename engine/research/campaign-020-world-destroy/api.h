#ifndef OP_WORLD_DESTROY_API_H
#define OP_WORLD_DESTROY_API_H
#include <stddef.h>
typedef struct OP_WORLD
{
    unsigned int loaded;
    unsigned char before_retained[0x9c];
    void *retained;
    unsigned char before_actor_count[0x144];
    unsigned int actor_count;
    unsigned char before_actor[8];
    void *actor;
    unsigned char remainder[0xae4];
} OP_WORLD;
typedef char op_world_extent[sizeof(OP_WORLD) == 0xcdc ? 1 : -1];
typedef char op_world_pointer[sizeof(void *) == 4 ? 1 : -1];
typedef char op_world_loaded[offsetof(OP_WORLD, loaded) == 0 ? 1 : -1];
typedef char op_world_retained[offsetof(OP_WORLD, retained) == 0xa0 ? 1 : -1];
typedef char op_world_actor_count[offsetof(OP_WORLD, actor_count) == 0x1e8 ? 1 : -1];
typedef char op_world_actor[offsetof(OP_WORLD, actor) == 0x1f4 ? 1 : -1];
void op_release(void *pointer);
void op_world_free_materials(OP_WORLD *world);
void op_world_free_pointer_table_220(OP_WORLD *world);
void op_world_free_pointer_table_620(OP_WORLD *world);
void op_world_free_tail_blocks(OP_WORLD *world);
void op_world_free_scripts(OP_WORLD *world);
int op_world_destroy(OP_WORLD *world);
#endif