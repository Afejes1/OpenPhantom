#include "api.h"
// FUNCTION: WMAIN 0x0043af45
OP_SHIELD *op_shield_set_offset(int slot, float x, float y, float z)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0;
    shield->offset_x = x;
    shield->offset_y = y;
    shield->offset_z = z;
    return shield;
}
