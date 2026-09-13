#include "effects_state.h"
// FUNCTION: WMAIN 0x00439370
void op_tint_start_full(int mode, float duration, int hold, unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a)
{
    op_tint_active = 1;
    op_tint_completed = 0;
    op_tint_mode = mode;
    op_tint_start = (float)op_clock_seconds();
    op_tint_duration = duration;
    op_tint_hold = hold;
    op_tint_red.value = r;
    op_tint_green.value = g;
    op_tint_blue.value = b;
    op_tint_alpha.value = a;
}
