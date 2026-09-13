#include "bgl_state.h"
#include <string.h>
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x0041f910
void op_bgl_load_matrix(const OP_BGL_MATRIX *source)
{
    memcpy(op_bgl_current, source, sizeof(*source));
}
