#include "bgl_state.h"
#include <string.h>
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x0041f929
void op_bgl_get_matrix(OP_BGL_MATRIX *destination)
{
    memcpy(destination, op_bgl_current, sizeof(*destination));
}
