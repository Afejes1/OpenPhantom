#include "api.h"
// FUNCTION: WMAIN 0x004396cf
void op_letterbox_set_enabled(int enabled)
{
    if (enabled != 0 && op_letterbox_previous == 0 && op_letterbox_target == 0)
        op_letterbox_step = 23;
    if (enabled == 0 && op_letterbox_previous != 0 && op_letterbox_target != 0)
        op_letterbox_step = 24;
    op_letterbox_target = enabled;
}
