#include "zap_effects.h"
// FUNCTION: WMAIN 0x0043ccf0
int op_zap_startup(void)
{
    if (op_zap_sprite == 0)
    {
        op_zap_sprite = op_acquire_sprite(op_zap_name);
        if (op_zap_sprite == 0)
            return 0;
    }
    return 1;
}
