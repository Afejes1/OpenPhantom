#include "api.h"
// FUNCTION: WMAIN 0x00439720
void op_fade_reset(void)
{
    op_tint_stop();
    op_letterbox_target = 0;
    op_letterbox_previous = 0;
    op_letterbox_step = 0;
}
