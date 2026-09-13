#include "api.h"

// FUNCTION: WMAIN 0x0041612f
void op_sound_release_handle(int index)
{
    if (op_sound_channels[index].is3d != 0) {
        op_sound_release_3d(op_sound_channels[index].sample3d);
        op_sound_channels[index].sample3d = 0;
    } else {
        op_sound_release_2d(op_sound_channels[index].sample);
        op_sound_channels[index].sample = 0;
    }
}