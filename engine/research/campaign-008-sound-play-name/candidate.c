#include "api.h"

// FUNCTION: WMAIN 0x00416787
int op_sound_play_name(const char *name, unsigned int flags)
{
    int result;

    if (op_sound_initialized == 0)
        return -1;
    op_sound_mode = 1;
    result = op_sound_play_descriptor_name(-1, name, 0, 0, flags);
    return result;
}
