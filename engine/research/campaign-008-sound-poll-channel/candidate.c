#include "api.h"

// FUNCTION: WMAIN 0x00415d8a
void op_sound_poll_channel(unsigned int index)
{
    int status;

    if (op_sound_channels[index].is3d != 0)
        status = op_sound_status3d(op_sound_channels[index].sample3d);
    else
        status = op_sound_status2d(op_sound_channels[index].sample);
    switch (status) {
    case 2:
        if (op_sound_channels[index].record != 0) {
            op_sound_channels[index].flags &= ~0x20000;
            op_sound_release_slot(index);
        }
        break;
    case 4:
        op_sound_update_slot(index, status);
        break;
    }
}
