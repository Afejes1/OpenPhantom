#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_translate_vector(const OP_VEC3 *input)
{
    OP_MATRIX matrix;
    op_build_translation(&matrix,input);
    op_multiply(op_bgl_current,&matrix);
}
