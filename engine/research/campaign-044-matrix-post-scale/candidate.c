#include "api.h"
// FUNCTION: WMAIN 0x0047e185
void op_matrix_post_scale(OP_MATRIX34 *matrix, OP_VECTOR3 *vector)
{
    OP_MATRIX34 temporary;
    op_matrix_build_scale(&temporary, vector);
    op_matrix_multiply_post(matrix, &temporary);
}
