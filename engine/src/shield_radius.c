#include "focused_accessors.h"
// FUNCTION: WMAIN 0x0043afd3
float op_shield_radius(int slot)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0.0f;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0.0f;
    return shield->radius;
}
