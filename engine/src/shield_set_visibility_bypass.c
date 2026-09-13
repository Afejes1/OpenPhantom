#include "focused_accessors.h"
// The +0x0c flag suppresses saves and bypasses view-volume classification.
// Stable research symbol retained; team name is fxshield_setNoSave.
// FUNCTION: WMAIN 0x0043b059
int op_shield_set_visibility_bypass(int slot, int value)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0;
    shield->no_save = value;
    return 1;
}
