#include "matrix34.h"
// FUNCTION: WMAIN 0x0047e0f7
void op_matrix_pre_rotate(OP_MATRIX34 *matrix, OP_VECTOR3 *vector)
{
    OP_MATRIX34 temporary;
    op_matrix_build_rotation(&temporary, vector);
    op_matrix_multiply_pre(matrix, &temporary);
}
