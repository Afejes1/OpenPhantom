#include "api.h"
// FUNCTION: WMAIN 0x0047c892
void op_vector_snap_small(OP_VECTOR3 *vector, float threshold)
{
    vector->x = (vector->x < op_geometry_zero ? -vector->x : vector->x) < threshold ? 0.0f : vector->x;
    vector->y = (vector->y < op_geometry_zero ? -vector->y : vector->y) < threshold ? 0.0f : vector->y;
    vector->z = (vector->z < op_geometry_zero ? -vector->z : vector->z) < threshold ? 0.0f : vector->z;
}
