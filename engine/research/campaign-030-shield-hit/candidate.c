#include "api.h"
#include <string.h>
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x0043b8ff
OP_HIT *op_shield_hit(int slot, OP_VEC3 *direction, float phase_turns)
{
    OP_SHIELD *current;
    unsigned int i;
    OP_HIT *payload;
    current = (OP_SHIELD *)((unsigned int)op_shields + (unsigned int)slot * sizeof(OP_SHIELD));
    if (slot < 0 || slot >= 32)
        return 0;
    if (!current->active)
        return 0;
    payload = current->hits;
    for (i = 0; i < current->hit_count; i++, payload++)
        if (!payload->active)
            break;
    if (i >= current->hit_count)
        return 0;
    payload->active = 1;
    memcpy(&payload->direction, direction, sizeof(OP_VEC3));
    op_normalize_vector(&payload->direction);
    op_reference_axis(&payload->basis, &payload->direction);
    op_identity_transform();
    op_rotate_transform(&payload->direction, phase_turns * op_degrees_per_turn);
    op_transform_point(&payload->basis);
    payload->state = 0;
    payload->lifetime = 2.0f;
    payload->progress = 0;
    payload->mode = 0;
    return payload;
}
