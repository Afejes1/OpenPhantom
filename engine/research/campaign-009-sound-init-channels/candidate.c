#include "api.h"
#include <string.h>
#pragma intrinsic(memset)

// FUNCTION: WMAIN 0x0041609e
void op_sound_init_channels(void)
{
    op_sound_channel *channel;
    int i;

    for (i = 0; i < 12; ++i) {
        op_sound_release_slot(i);
        channel = &op_sound_channels[i];
        op_sound_release_handle(i);
        memset(channel, 0, sizeof(*channel));
        channel->sample = op_sound_allocate_sample(op_sound_driver);
        op_sound_init_sample(channel->sample);
        channel->flags |= 0x80000;
        channel->slot = i;
    }
}
