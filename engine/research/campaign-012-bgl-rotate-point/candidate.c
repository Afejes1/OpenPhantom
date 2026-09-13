#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_rotate_point(OP_VEC3 *point)
{
    OP_VEC3 value;
    op_rotate_basis(&value,point,op_bgl_current);
    *point=value;
}
