#include "api.h"

// FUNCTION: WMAIN 0x004172b4
void op_sound_play_voice(const char *name, int *handle, float *position)
{
    unsigned int flags;

    if (handle != 0 && *handle >= 0)
        op_sound_release_slot(*handle);
    if (position != 0) {
        op_sound_mode = 3;
        op_sound_set_field(2, 100.0f);
        op_sound_set_field(4, 100.0f);
        if (op_sound_has_input_lock(5) != 0)
            op_sound_set_field(3, 8.0f);
        else
            op_sound_set_field(3, 4.0f);
        flags = 0x4a04;
    } else {
        op_sound_mode = 4;
        flags = 0x4201;
    }
    op_sound_play_descriptor_name(0, name, handle, position, flags);
}
