#include "api.h"

// FUNCTION: WMAIN 0x00415d1d
int op_sound_periodic(void)
{
    int i;

    if (op_sound_initialized == 0)
        return 1;
    op_sound_mode = -1;
    if (op_sound_world_enabled == 1) {
        op_sound_update_listener();
        op_sound_update_placements();
    }
    if (op_sound_paused == 1)
        return 1;
    for (i = 0; i < 12; ++i)
        op_sound_poll_channel(i);
    return 0;
}
