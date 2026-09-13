#include "api.h"

// FUNCTION: WMAIN 0x004178fa
int op_sound_is_muted(void)
{
    if (op_sound_disabled == 1)
        return 1;
    if (op_sound_initialized == 0)
        return 1;
    return 0;
}
