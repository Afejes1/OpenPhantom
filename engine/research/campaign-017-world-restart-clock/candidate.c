#include "api.h"
// FUNCTION: WMAIN 0x0041eb5a
void op_world_restart_clock(OP_WORLD *world)
{
    if (world == 0) return;
    world->timestamp = op_clock_ticks();
    world->mirror = world->timestamp;
    world->elapsed = 0;
    op_world_clock_hook(world);
}
