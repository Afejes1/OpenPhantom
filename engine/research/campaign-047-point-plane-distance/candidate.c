#include "api.h"
// FUNCTION: WMAIN 0x0047c686
float op_point_plane_distance(const OP_VECTOR3 *point, const OP_VECTOR3 *normal, const OP_VECTOR3 *origin)
{
    return (point->x - origin->x) * normal->x + (point->y - origin->y) * normal->y + (point->z - origin->z) * normal->z;
}
