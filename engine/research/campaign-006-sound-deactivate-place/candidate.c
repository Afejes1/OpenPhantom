#include "api.h"

// FUNCTION: WMAIN 0x0041778c
void op_sound_deactivate_place(op_sound_placement *placement)
{
    if (op_sound_initialized == 0)
        return;
    if (placement == 0)
        return;
    placement->active = 0;
}