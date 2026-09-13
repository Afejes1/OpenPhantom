#include "api.h"

// FUNCTION: WMAIN 0x004177ae
void op_sound_control_place(int index, int enabled)
{
    op_sound_placement *placement;

    if (op_sound_initialized == 0)
        return;
    if (op_sound_world != 0 && op_sound_world->placements != 0 &&
        index >= 0 && index < op_sound_world->placement_count) {
        placement = &op_sound_world->placements[index];
        if (enabled != 0)
            op_sound_activate_place(placement);
        else
            op_sound_deactivate_place(placement);
    }
}