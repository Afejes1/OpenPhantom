#include "api.h"

// FUNCTION: WMAIN 0x00416634
void op_sound_reset_places(void)
{
    op_sound_placement *placement;
    int i;

    if (op_sound_world != 0) {
        placement = op_sound_world->placements;
        for (i = 0; i < op_sound_world->placement_count; ++i) {
            placement->active = placement->initial_active;
            placement->deadline = 0.0f;
        }
    }
}