#include "baseline.h"

// FUNCTION: WMAIN 0x0040dcee
/* VC5-calibrated source. Valid axis is 0, 1 or 2.
 * Double evaluation in the Y/Z divisions retains the observed x87 load order.
 * These separate stores and the switch are observable in the original codegen.
 * VC5's x87 equality sequence also takes the early return for unordered input;
 * a modern compiler's ordered C equality does not reproduce that behavior.
 */
float op_plane_coordinate(int axis, float *vertex, float *normal, float *point)
{
    if (normal[axis] == 0.0f)
        return vertex[axis];
    if (normal[axis] == 1.0f)
        return vertex[axis];
    switch (axis) {
    case 0:
        point[0] = (vertex[1] - point[1]) * normal[1];
        point[0] = (vertex[2] - point[2]) * normal[2] + point[0];
        point[0] = point[0] / normal[0];
        point[0] = point[0] + vertex[0];
        break;
    case 1:
        point[1] = (vertex[0] - point[0]) * normal[0];
        point[1] = (vertex[2] - point[2]) * normal[2] + point[1];
        point[1] = (float)((double)point[1] / normal[1]);
        point[1] = point[1] + vertex[1];
        break;
    case 2:
        point[2] = (vertex[0] - point[0]) * normal[0];
        point[2] = (vertex[1] - point[1]) * normal[1] + point[2];
        point[2] = (float)((double)point[2] / normal[2]);
        point[2] = point[2] + vertex[2];
        break;
    }
    return point[axis];
}
