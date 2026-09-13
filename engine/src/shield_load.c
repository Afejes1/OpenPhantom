#include "effects_save.h"
#include "focused_accessors.h"
// FUNCTION: WMAIN 0x0043cafc
int op_shield_load(void)
{
    OP_SHIELD *cursor;
    int header_count;
    int slot;
#if defined(_MSC_VER) && _MSC_VER == 1100
    int count;
#else
    /* Supplemental compiler cannot prove the first iteration after the positive-count gate. */
    int count = 0;
#endif
    OP_SHIELD_SAVE payload;
    if (!op_save_read(&header_count, 4))
        return 1;
    if (header_count <= 0)
        return 0;
    for (slot = 0; slot < header_count; slot++)
    {
        op_save_read(&payload, 52);
        count = op_shield_allocate(0);
        if (count < 0)
            break;
        cursor = &op_shields[count];
        cursor->saved_actor = payload.slot;
        cursor->visible = payload.visible;
        cursor->no_save = 0;
        cursor->elapsed = payload.elapsed;
        if (payload.name[0])
            op_shield_set_texture(count, payload.name);
        cursor->radius = payload.radius;
        cursor->red = (unsigned char)((payload.color >> 16) & 255);
        cursor->green = (unsigned char)((payload.color >> 8) & 255);
        cursor->blue = (unsigned char)(payload.color & 255);
        cursor->alpha = (unsigned char)(payload.color >> 24);
    }
    if (count < 0 || slot < header_count)
        return 1;
    return 0;
}
