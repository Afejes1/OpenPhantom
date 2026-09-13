#include "api.h"
// FUNCTION: WMAIN 0x0043a44e
void op_ripple_tick(void)
{
    OP_RIPPLE *cursor;
    int slot;
    float spread;
    float length;
    float amplitude;
    OP_VEC3 delta;
    for (slot = 0, cursor = op_ripples; slot < 256; slot++, cursor++)
    {
        if (!cursor->active)
            continue;
        length = op_effect_seconds - cursor->time;
        if (length < op_float_zero)
            length = 0;
        if (cursor->last_update != op_float_zero && !cursor->moving)
            continue;
        delta.x = cursor->velocity.x * length + cursor->position.x;
        delta.y = cursor->velocity.y * length + cursor->position.y;
        delta.z = cursor->velocity.z * length + cursor->position.z;
        spread = cursor->radial_velocity * length + cursor->radius;
        op_remove_projection_group(cursor->token);
        if (cursor->lifetime == op_float_zero)
            amplitude = 1.0f;
        else
        {
            amplitude = op_float_one - length / cursor->lifetime;
            if (amplitude < op_float_zero)
                amplitude = 0;
            else if (amplitude > op_float_one)
                amplitude = 1.0f;
        }
        op_render_ripple(&delta, spread, cursor->yaw, cursor->appearance, cursor->render_parameter, cursor->token,
                         amplitude, cursor->required_type, -101);
        cursor->last_update = op_effect_seconds;
        if (length >= cursor->lifetime || cursor->lifetime == op_float_zero)
        {
            cursor->active = 0;
            continue;
        }
    }
}
