#include "api.h"
// FUNCTION: WMAIN 0x00495c6f
void op_music_set_volume(float value)
{
    unsigned int volume;
    if (!op_music_gate || op_music_aux_device < 0)
        return;
    volume = (int)(value * 65535.0f);
    volume |= (volume & 0xffffu) << 16;
    op_music_aux_volume((unsigned int)op_music_aux_device, volume);
}
