#include "api.h"

// FUNCTION: WMAIN 0x004167c3
int op_sound_play_name_volume(const char *name, float volume,
                              unsigned int flags)
{
    int result;

    if (op_sound_initialized == 0)
        return -1;
    op_sound_set_field(0, volume);
    op_sound_mode = 2;
    result = op_sound_play_descriptor_name(-1, name, 0, 0, flags | 0x800);
    op_sound_set_field(0, 2.0f);
    return result;
}
