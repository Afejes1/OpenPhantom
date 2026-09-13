#include "api.h"
// FUNCTION: WMAIN campaign-012
#include <string.h>
#pragma intrinsic(memcpy)
void op_bgl_scale_xyz(float x,float y,float z)
{
    OP_MATRIX matrix;
    memcpy(&matrix,&op_bgl_identity_matrix,sizeof(matrix));
    matrix.basis[0]=x;matrix.basis[4]=y;matrix.basis[8]=z;
    op_multiply(op_bgl_current,&matrix);
}
