#include "geometry.h"
// FUNCTION: WMAIN 0x0047c6c2
float op_vector_angle(const OP_VECTOR3 *a, const OP_VECTOR3 *b)
{
    union {
        float value;
        unsigned int bits;
    } dot;
    dot.value = (float)(a->x * b->x + (double)a->y * b->y + (double)a->z * b->z);
    if (dot.bits == 0x3f800000u)
        return op_geometry_zero;
    return op_geometry_right_angle - op_scalar_angle(dot.value);
}
