#include "api.h"
// FUNCTION: WMAIN 0x0043b016
int op_shield_set_visible(int slot, int value)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->owner == 0)
        return 0;
    shield->visible = value;
    return 1;
}
