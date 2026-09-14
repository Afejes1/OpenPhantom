#include "debug_helpers.h"
// FUNCTION: WMAIN 0x0046ac63
void op_debug_toggle_size(void)
{
    if (op_debug_size == 1)
        op_debug_resize(0, 2048, 2048);
    else
        op_debug_resize(0, 200, 48);
    op_debug_size = op_debug_size == 0;
}
