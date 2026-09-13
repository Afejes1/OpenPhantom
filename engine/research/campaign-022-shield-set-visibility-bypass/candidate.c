#include "api.h"
// The +0x0c flag suppresses saves and bypasses view-volume classification.
// Stable research symbol retained; team name is fxshield_setNoSave.
// FUNCTION: WMAIN 0x0043b059
int op_shield_set_visibility_bypass(int slot, int value)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->owner == 0)
        return 0;
    shield->visibility_bypass = value;
    return 1;
}
