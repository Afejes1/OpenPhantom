#include "api.h"

// FUNCTION: WMAIN 0x004178a3
void op_sound_disable(void)
{
    if (op_sound_disabled == 1)
        return;
    op_sound_write_option(op_sound_disabled_key, 1);
    op_sound_stop_all();
    op_sound_disabled = 1;
}