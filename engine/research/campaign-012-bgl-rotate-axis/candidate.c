#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_rotate_axis(const OP_VEC3 *axis,float angle)
{
    OP_MATRIX matrix;
    if (angle == 0.0f)
        return;
    op_build_rotation(&matrix,axis,angle);
    op_multiply(op_bgl_current,&matrix);
}
