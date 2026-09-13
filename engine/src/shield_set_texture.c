#include "focused_accessors.h"
// FUNCTION: WMAIN 0x0043ae53
#if defined(_MSC_VER) && _MSC_VER == 1100
#include <string.h>
#pragma intrinsic(strcpy)
#endif
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
#if defined(_MSC_VER) && _MSC_VER == 1100
    strcpy(shield->name, name);
#else
    /* Supplemental modern build: preserve the observed unbounded NUL copy. */
    {
        char *destination = shield->name;
        char *source = name;
        char copied;
        do
        {
            copied = *source;
            *destination = copied;
            ++source;
            ++destination;
        } while (copied != 0);
    }
#endif
    return 1;
}
