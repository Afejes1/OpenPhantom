#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_transform_xyz(float *x,float *y,float *z)
{
    OP_VEC3 value;
    value.x=*x;value.y=*y;value.z=z!=0?*z:0.0f;
    op_bgl_transform_point(&value);
    *x=value.x;*y=value.y;
    if(z!=0)*z=value.z;
}
