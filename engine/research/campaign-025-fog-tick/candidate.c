#include "api.h"
// FUNCTION: WMAIN 0x00438f81
void op_fog_tick(float delta)
{
    float value;
    if (op_fog_remaining > 0.0f)
    {
        op_fog_remaining -= delta;
        if (op_fog_remaining <= 0.0f)
        {
            op_fog_cached_start = op_fog_target;
            if (op_fog_restore_mode != 0)
                op_fog_restore();
        }
        else
        {
            value = op_fog_target - ((op_fog_target - op_fog_cached_start) * op_fog_remaining) / op_fog_duration;
            op_set_render_flags(op_get_render_flags() & ~0x40U);
            op_renderer_fog_start(value);
            op_set_render_flags(op_get_render_flags() | 0x40U);
        }
    }
}
