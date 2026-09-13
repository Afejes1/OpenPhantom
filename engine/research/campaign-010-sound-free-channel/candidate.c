#include "api.h"

// FUNCTION: WMAIN 0x00417567
void op_sound_free_channel(unsigned int slot)
{
    op_sound_channel *channel;

    if (slot >= 12)
        return;
    channel = &op_sound_channels[slot];
    if ((channel->flags & 0x20000) != 0) {
        if (channel->type != 0)
            op_sound_end3d(channel->sample3d);
        else
            op_sound_end2d(channel->sample);
    }
    if (channel->caller_handle != 0) {
        *channel->caller_handle = -1;
        channel->caller_handle = 0;
    }
    if (channel->record != 0) {
        if ((channel->flags & 0x4000) != 0)
            op_sound_mark_resource(channel->record->resource, 1);
        op_sound_release_resource(channel->record->resource);
        channel->record->resource = 0;
        channel->record = 0;
    }
    channel->position_owner = 0;
    channel->flags = 0x80000;
    channel->state40 = 0;
    channel->state70 = 0;
}
