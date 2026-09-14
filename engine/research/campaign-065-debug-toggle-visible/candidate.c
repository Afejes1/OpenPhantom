#include "api.h"
// FUNCTION: WMAIN 0x0046a810
void op_debug_toggle_visible(void)
{
    op_debug_visible++;
    if ((int)op_debug_visible > 1)
    {
        op_debug_reset(1);
        op_debug_visible = 0;
    }
}
