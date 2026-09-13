#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_transform_point(OP_VEC3 *point)
{
    OP_VEC3 value;
    op_rotate_basis(&value,point,op_bgl_current);
    value.x+=op_bgl_current->translation.x;
    value.y+=op_bgl_current->translation.y;
    value.z+=op_bgl_current->translation.z;
    *point=value;
}
