#include "api.h"

// FUNCTION: WMAIN 0x00417826
void op_sound_pin_channel(int index, const op_vec3 *position)
{
    op_sound_channel *channel;

    channel = &op_sound_channels[index];
    if (channel->active != 0) {
        channel->flags |= 0x20;
        channel->position = *position;
    }
}