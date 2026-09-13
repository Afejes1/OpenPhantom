#include "api.h"
#include <string.h>
#pragma intrinsic(memset)

// FUNCTION: WMAIN 0x0041faa9
void op_bgl_perp_axis(OP_VEC3 *output, const OP_VEC3 *input)
{
    OP_MATRIX matrix;
    OP_VEC3 axis;
    float minimum = 2.0f;
    int i;
    int selected = 0;

    for (i = 0; i < 3; ++i) {
        if ((input->v[i] < 0.0f ? -input->v[i] : input->v[i]) < minimum) {
            selected = i;
            minimum = input->v[i] < 0.0f ? -input->v[i] : input->v[i];
        }
    }

    memset(&axis, 0, sizeof(axis));
    axis.v[selected] = 1.0f;
    op_build_rotation(&matrix, &axis, 90.0f);
    op_rotate_basis(output, &axis, &matrix);
}
