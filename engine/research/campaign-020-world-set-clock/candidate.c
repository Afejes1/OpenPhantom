#include "api.h"
// FUNCTION: WMAIN 0x0041f0c9
void op_world_set_clock(OP_WORLD *world, float seconds)
{
    if (world != 0) {
        world->previous_ticks = world->current_ticks;
        world->current_ticks = (int)(seconds * 1000.0f);
        world->delta = seconds - world->seconds;
        world->seconds = seconds;
    }
}