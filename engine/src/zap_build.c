#include "zap_effects.h"
#include <math.h>
#pragma intrinsic(sqrt)
// FUNCTION: WMAIN 0x0043d6b1
void op_generate_zap_points(OP_VEC3 *start, OP_VEC3 *end, int depth)
{
    int last;
    float spread;
    float magnitude;
    OP_VEC3 offset;
    op_zap_points[0].x = start->x;
    op_zap_points[0].y = start->y;
    op_zap_points[0].z = start->z;
    last = 1 << depth;
    op_zap_points[last].x = end->x;
    op_zap_points[last].y = end->y;
    op_zap_points[last].z = end->z;
    offset.x = start->x - end->x;
    offset.y = start->y - end->y;
    offset.z = start->z - end->z;
    magnitude = (float)sqrt(offset.z * offset.z + (offset.y * offset.y + offset.x * offset.x));
    spread = magnitude * op_zap_amplitude_scale;
    op_subdivide_zap(0, last, depth, spread);
}
