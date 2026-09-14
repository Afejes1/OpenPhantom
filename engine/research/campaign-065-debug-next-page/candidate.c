#include "api.h"
// FUNCTION: WMAIN 0x0046a83f
void op_debug_next_page(void)
{
    op_debug_visible = 1;
    op_debug_pulse = 0;
    op_debug_page++;
    if (op_debug_page == 4)
    {
        op_debug_reset(1);
        op_debug_visible = 0;
    }
    if ((int)op_debug_page > 4)
        op_debug_page = 0;
}
