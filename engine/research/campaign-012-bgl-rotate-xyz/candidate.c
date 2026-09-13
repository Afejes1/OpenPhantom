#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_rotate_xyz(float x,float y,float z,float angle)
{
    OP_VEC3 axis;
    axis.x=x;axis.y=y;axis.z=z;
    op_bgl_rotate_axis(&axis,angle);
}
