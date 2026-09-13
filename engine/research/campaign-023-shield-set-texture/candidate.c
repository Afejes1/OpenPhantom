#include "api.h"
// FUNCTION: WMAIN 0x0043ae53
#include <string.h>
#pragma intrinsic(strcpy)
int op_shield_set_texture(int slot, char *name)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0;
    op_release_sprite(&shield->sprite);
    shield->sprite = op_acquire_sprite(name);
    if (shield->sprite == 0)
        return 0;
    strcpy(shield->name, name);
    return 1;
}
