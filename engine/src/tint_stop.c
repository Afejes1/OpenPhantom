#include "effects_state.h"
// FUNCTION: WMAIN 0x004393fa
void op_tint_stop(void)
{
    if (op_tint_active == 0)
        return;
    op_tint_active = 0;
    op_tint_mode = 0;
    op_tint_start = 0.0f;
    op_tint_duration = 0.0f;
}
