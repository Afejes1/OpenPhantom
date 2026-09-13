#include "api.h"

// FUNCTION: WMAIN 0x00417143
void op_sound_play_index(unsigned int index, int *handle, float *position)
{
    op_sound_descriptor *descriptor;

    if (index >= op_sound_world->descriptor_count)
        return;
    descriptor = op_sound_world->descriptors;
    descriptor += index;
    if (op_sound_mode < 0)
        op_sound_mode = 8;
    op_sound_start_descriptor(descriptor, handle, position);
}
