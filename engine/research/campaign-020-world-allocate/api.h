#ifndef OP_WORLD_ALLOCATE_API_H
#define OP_WORLD_ALLOCATE_API_H
#include <stddef.h>
/* Known fields within the independently observed 0xCDC-byte allocation. */
typedef struct OP_WORLD
{
    unsigned char before_clock[0x50];
    int current_ticks;
    float seconds;
    int previous_ticks;
    float delta;
    float ambient;
    unsigned char before_capacity[0x14];
    unsigned int capacity;
    unsigned int capacity_state;
    unsigned char before_texture_capacity[0x40];
    unsigned int texture_capacity;
    unsigned char before_fog[0x14c];
    unsigned int fog_flags;
    unsigned int fog_color;
    float fog_start;
    float fog_end;
    unsigned char after_fog[0xabc];
} OP_WORLD;
typedef char op_world_extent[sizeof(OP_WORLD) == 0xcdc ? 1 : -1];
typedef char op_world_word[sizeof(unsigned int) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef char op_world_clock_offset[offsetof(OP_WORLD, current_ticks) == 0x50 ? 1 : -1];
typedef char op_world_seconds_offset[offsetof(OP_WORLD, seconds) == 0x54 ? 1 : -1];
typedef char op_world_previous_offset[offsetof(OP_WORLD, previous_ticks) == 0x58 ? 1 : -1];
typedef char op_world_delta_offset[offsetof(OP_WORLD, delta) == 0x5c ? 1 : -1];
typedef char op_world_ambient_offset[offsetof(OP_WORLD, ambient) == 0x60 ? 1 : -1];
typedef char op_world_capacity_offset[offsetof(OP_WORLD, capacity) == 0x78 ? 1 : -1];
typedef char op_world_state_offset[offsetof(OP_WORLD, capacity_state) == 0x7c ? 1 : -1];
typedef char op_world_texture_offset[offsetof(OP_WORLD, texture_capacity) == 0xc0 ? 1 : -1];
typedef char op_world_fog_flags_offset[offsetof(OP_WORLD, fog_flags) == 0x210 ? 1 : -1];
typedef char op_world_fog_color_offset[offsetof(OP_WORLD, fog_color) == 0x214 ? 1 : -1];
typedef char op_world_fog_start_offset[offsetof(OP_WORLD, fog_start) == 0x218 ? 1 : -1];
typedef char op_world_fog_end_offset[offsetof(OP_WORLD, fog_end) == 0x21c ? 1 : -1];
void *op_allocate(unsigned int bytes);
int op_milliseconds(void);
OP_WORLD *op_world_allocate(void);
#endif
