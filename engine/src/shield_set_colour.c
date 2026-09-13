#include "focused_accessors.h"
// FUNCTION: WMAIN 0x0043aee7
int op_shield_set_colour(int slot, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->owner == 0)
        return 0;
    shield->red = red;
    shield->green = green;
    shield->blue = blue;
    shield->alpha = alpha;
    return 1;
}
