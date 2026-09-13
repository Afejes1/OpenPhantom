#include "api.h"
// FUNCTION: WMAIN 0x0043906e
void op_fog_start(int duration, float target, int red, int green, int blue, int restore)
{
    if (duration <= 0)
        return;
    if (target < 0.0f)
        return;
    op_fog_restore_mode = restore;
    if (op_fog_restore_mode == 0)
    {
        op_set_render_flags(op_get_render_flags() & ~0x40U);
        op_set_fog_rgb(red & 255, green & 255, blue & 255);
        op_set_clear_rgb(0, 0, 0);
        op_set_render_flags(op_get_render_flags() | 0x40U);
        op_active_world->flags |= 1;
    }
    op_fog_target = target;
    op_fog_duration = (float)duration;
    op_fog_remaining = op_fog_duration;
}
