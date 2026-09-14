#include "api.h"
// FUNCTION: WMAIN 0x0047e1af
void op_matrix_pre_scale(OP_MATRIX34 *matrix, OP_VECTOR3 *vector)
{
    OP_MATRIX34 temporary;
    op_matrix_build_scale(&temporary, vector);
    op_matrix_multiply_pre(matrix, &temporary);
}
