#ifndef OP_WORLD_SET_CLOCK_API_H
#define OP_WORLD_SET_CLOCK_API_H
#include <stddef.h>
typedef struct OP_WORLD
{
    unsigned char before_clock[0x50];
    int current_ticks;
    float seconds;
    int previous_ticks;
    float delta;
    unsigned char remainder[0xc7c];
} OP_WORLD;
typedef char op_clock_extent[sizeof(OP_WORLD) == 0xcdc ? 1 : -1];
typedef char op_clock_words[sizeof(int) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef char op_clock_current[offsetof(OP_WORLD, current_ticks) == 0x50 ? 1 : -1];
typedef char op_clock_seconds[offsetof(OP_WORLD, seconds) == 0x54 ? 1 : -1];
typedef char op_clock_previous[offsetof(OP_WORLD, previous_ticks) == 0x58 ? 1 : -1];
typedef char op_clock_delta[offsetof(OP_WORLD, delta) == 0x5c ? 1 : -1];
void op_world_set_clock(OP_WORLD *world, float seconds);
#endif