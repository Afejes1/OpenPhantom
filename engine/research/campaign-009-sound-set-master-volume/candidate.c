#include "api.h"

// FUNCTION: WMAIN 0x00417379
void op_sound_set_master_volume(int value)
{
    op_sound_channel *channel;
    float volume;
    int i;

    if (op_sound_initialized == 0)
        return;
    op_sound_master_gain = (float)value / 127.0f;
    for (i = 0; i < 12; ++i) {
        channel = &op_sound_channels[i];
        if (channel->type == 1) {
            volume = channel->volume * 127.0f;
            volume *= op_sound_master_gain;
            volume = volume >= 0.0f ?
                     (volume <= 127.0f ? volume : 127.0f) : 0.0f;
            op_sound_set_volume3d(channel->sample3d, (int)volume);
        }
    }
    op_sound_set_master(op_sound_driver, value);
}
