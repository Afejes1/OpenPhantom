#include "api.h"
#include <string.h>
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x0043c9b1
int op_shield_write(void)
{
    OP_SHIELD *cursor;
    int header_count;
    int slot;
    int count = 0;
    OP_SHIELD_SAVE payload;
    for (slot = 0; slot < 32; slot++)
    {
        cursor = &op_shields[slot];
        if (cursor->active && !cursor->stopping)
            count++;
    }
    header_count = count;
    op_save_write(&header_count, 4);
    if (count <= 0)
        return 0;
    for (slot = 0; slot < 32; slot++)
    {
        cursor = &op_shields[slot];
        if (!cursor->active || cursor->stopping || cursor->no_save)
            continue;
        count--;
        payload.slot = slot;
        payload.visible = cursor->visible;
        payload.elapsed = cursor->elapsed;
        memcpy(payload.name, cursor->name, 32);
        payload.name[31] = 0;
        payload.radius = cursor->radius;
        payload.color = ((unsigned int)cursor->alpha << 24) | ((unsigned int)cursor->red << 16) |
                        ((unsigned int)cursor->green << 8) | cursor->blue;
        op_save_write(&payload, 52);
    }
    return 0;
}
