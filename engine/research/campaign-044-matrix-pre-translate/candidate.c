#include "api.h"
// FUNCTION: WMAIN 0x0047e14b
void op_matrix_pre_translate(OP_MATRIX34 *matrix, OP_VECTOR3 *vector)
{
    matrix->translation.x += vector->x;
    matrix->translation.y += vector->y;
    matrix->translation.z += vector->z;
}
