#include "api.h"

// FUNCTION: WMAIN 0x004178d1
void op_sound_enable(void)
{
    if (op_sound_disabled == 0)
        return;
    op_sound_write_option(op_sound_disabled_key, 0);
    op_sound_disabled = 0;
}