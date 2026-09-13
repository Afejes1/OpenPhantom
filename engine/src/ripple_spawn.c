#include "ripple_spawn.h"
#include <string.h>
#pragma intrinsic(memset, memcpy)
// FUNCTION: WMAIN 0x0043a2f0
int op_ripple_spawn(int appearance, OP_VEC3 *position, OP_VEC3 *velocity, float yaw, float radius,
                    float radial_velocity, float render_parameter, float lifetime, int required_type)
{
    OP_RIPPLE *current;
    int i;
    if (!op_ripples_enabled)
        return 0;
    if (!op_projection_enabled)
        return 0;
    for (i = 0; i < 256; i++)
        if (!op_ripples[i].active)
            break;
    if (i >= 256)
        return 0;
    current = &op_ripples[i];
    current->active = 1;
    current->appearance = appearance;
    memcpy(&current->position, position, sizeof(OP_VEC3));
    current->moving = 0;
    if (velocity)
    {
        memcpy(&current->velocity, velocity, sizeof(OP_VEC3));
        current->moving = 1;
    }
    else
        memset(&current->velocity, 0, sizeof(OP_VEC3));
    current->yaw = yaw;
    current->radius = radius;
    current->radial_velocity = radial_velocity;
    if (radial_velocity != op_float_zero)
        current->moving = 1;
    current->lifetime = lifetime;
    current->render_parameter = render_parameter;
    current->time = op_effect_seconds;
    current->state = 0;
    current->token = op_allocate_projection_token();
    current->required_type = required_type;
    return current->token;
}
