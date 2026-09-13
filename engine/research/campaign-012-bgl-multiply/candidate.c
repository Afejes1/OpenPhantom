#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_multiply(const OP_MATRIX *rhs)
{
    op_multiply(op_bgl_current,rhs);
}
