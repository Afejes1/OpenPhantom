#include "api.h"

// FUNCTION: WMAIN 0x00417659
void op_sound_start_sample(unsigned int index)
{
    op_sound_channel *channel;

    if (index >= 12)
        return;
    channel = &op_sound_channels[index];
    if (channel->type == 0) {
        if ((channel->flags & 0x10) != 0)
            op_sound_loop2d(channel->sample, 0);
        op_sound_start2d(channel->sample);
    } else {
        if ((channel->flags & 0x10) != 0)
            op_sound_loop3d(channel->sample3d, 0);
        op_sound_start3d(channel->sample3d);
    }
    channel->flags |= 0x20000;
}
