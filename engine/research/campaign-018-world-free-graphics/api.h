#ifndef OP_CAMPAIGN_018_WORLD_FREE_GRAPHICS_API_H
#define OP_CAMPAIGN_018_WORLD_FREE_GRAPHICS_API_H
#include <stddef.h>
/* Partial prefix; opaque bytes are owned fixture observation storage. */
typedef struct OP_WORLD {
    int load_state;
    unsigned char opaque[60];
} OP_WORLD;
typedef char op_graphics_load_offset[(offsetof(OP_WORLD, load_state) == 0) ? 1 : -1];
typedef char op_graphics_word_width[(sizeof(int) == 4) ? 1 : -1];
void op_world_free_materials(OP_WORLD *world);
void op_world_free_pointer_table_220(OP_WORLD *world);
void op_world_free_pointer_table_620(OP_WORLD *world);
int op_world_free_graphics(OP_WORLD *world);
#endif
