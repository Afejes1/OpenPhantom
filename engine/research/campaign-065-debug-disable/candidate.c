#include "api.h"
// FUNCTION: WMAIN 0x0046ad32
void op_debug_disable(void)
{
    op_debug_pulse = 0;
    op_debug_visible = 0;
}
