#include "api.h"
// FUNCTION: WMAIN 0x0041eb5a
void op_world_restart_clock(OP_B3D_WORLD *world)
{
    if (world == 0) return;
    world->current_ticks = op_milliseconds();
    world->previous_ticks = world->current_ticks;
    world->delta = 0.0f;
    op_world_mangle_names(world);
}
