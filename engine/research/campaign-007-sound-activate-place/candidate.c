#include "api.h"

// FUNCTION: WMAIN 0x00417711
void op_sound_activate_place(op_sound_placement *placement)
{
    if (op_sound_initialized == 0)
        return;
    if (placement == 0 || placement->active != 0 || op_sound_world == 0)
        return;
    placement->active = 1;
    placement->deadline = op_sound_world->time;
    if (placement->random_span != 0.0f) {
        placement->deadline += (float)op_sound_random() *
                               (1.0f / 32767.0f) *
                               placement->random_span;
    }
}