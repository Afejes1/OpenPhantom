#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_translate_xyz(float x,float y,float z)
{
    OP_VEC3 value;
    value.x=x;value.y=y;value.z=z;
    op_bgl_translate_vector(&value);
}
