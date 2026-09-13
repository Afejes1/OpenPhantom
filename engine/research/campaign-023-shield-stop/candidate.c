#include "api.h"
// FUNCTION: WMAIN 0x0043b5e6
int op_shield_stop(int slot)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0;
    shield->stopping = 1;
    shield->elapsed = 0.0f;
    if (shield->attached != 0)
        shield->attached->shield_slot = -1;
    return 1;
}
