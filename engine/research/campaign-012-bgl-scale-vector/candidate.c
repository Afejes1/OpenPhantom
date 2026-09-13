#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_scale_vector(const OP_VEC3 *input)
{
    op_bgl_scale_xyz(input->x,input->y,input->z);
}
