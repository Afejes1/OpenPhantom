#include "api.h"

// FUNCTION: WMAIN 0x004176e3
void op_sound_stop_all(void)
{
    int i;

    for (i = 0; i < 12; ++i)
        op_sound_release_slot(i);
}