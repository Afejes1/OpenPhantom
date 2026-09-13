#include "focused_accessors.h"
// FUNCTION: WMAIN 0x0043b67b
void op_shield_draw_released(void)
{
    OP_SHIELD *current;
    int i;
    if (op_shield_count <= 0)
        return;
    for (i = 0, current = op_shields; i < 32; ++i, ++current)
    {
        if (current->active == 0)
            continue;
        if (current->stopping == 0)
            continue;
        op_shield_draw(i);
        ++op_shield_pass;
    }
    op_shield_pass = 0;
    op_shield_other_pass = 0;
}
