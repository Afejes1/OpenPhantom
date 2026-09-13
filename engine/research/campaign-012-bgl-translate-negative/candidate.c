#include "api.h"
// FUNCTION: WMAIN campaign-012
void op_bgl_translate_negative(const OP_VEC3 *input)
{
    OP_VEC3 value;
    value.x=-input->x;value.y=-input->y;value.z=-input->z;
    op_bgl_translate_vector(&value);
}
