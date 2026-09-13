#include "api.h"

// FUNCTION: WMAIN 0x00417477
void op_sound_pause_all(void)
{
    op_sound_channel *channel;
    int i;

    for (i = 0; i < 12; ++i) {
        channel = &op_sound_channels[i];
        if ((channel->flags & 0x20000) != 0) {
            channel->flags |= 0x40000;
            if (channel->is3d == 0)
                op_sound_pause_2d(channel->sample);
            else
                op_sound_pause_3d(channel->sample3d);
        }
    }
}