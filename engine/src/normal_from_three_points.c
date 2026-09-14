#include "geometry.h"
// FUNCTION: WMAIN 0x0047c5b0
void op_normal_from_three_points(OP_VECTOR3 *out, const OP_VECTOR3 *current, const OP_VECTOR3 *next,
                                 const OP_VECTOR3 *previous)
{
    OP_VECTOR3 first, second;
    first.x = next->x - current->x;
    first.y = next->y - current->y;
    first.z = next->z - current->z;
    second.x = previous->x - current->x;
    second.y = previous->y - current->y;
    second.z = previous->z - current->z;
    op_vector_normalize(&first);
    op_vector_normalize(&second);
    out->x = first.y * second.z - first.z * second.y;
    out->y = first.z * second.x - first.x * second.z;
    out->z = first.x * second.y - first.y * second.x;
    op_vector_normalize(out);
    op_vector_snap_small(out, 0.000001f);
}
