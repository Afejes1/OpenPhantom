#include <string.h>
#include "api.h"
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0041cbbf
OP_WORLD *op_world_allocate(void)
{
    OP_WORLD *world;
    world = (OP_WORLD *)op_allocate(0xcdc);
    if (world == 0) return world;
    memset(world, 0, 0xcdc);
    world->current_ticks = op_milliseconds();
    world->seconds = 0.0f;
    world->previous_ticks = world->current_ticks;
    world->delta = 0.0f;
    world->capacity = 1024;
    world->capacity_state = 0;
    world->ambient = 0.1f;
    world->fog_flags = 0;
    world->fog_start = 10.0f;
    world->fog_end = 22.0f;
    world->fog_color = 0x408020;
    world->texture_capacity = 32;
    return world;
}