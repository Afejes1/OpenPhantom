#include "api.h"
#include <string.h>
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x0041f71b
void op_bgl_push(void)
{
    OP_BGL_MATRIX *previous;

    if (op_bgl_depth >= 63)
        return;
    previous = &op_bgl_stack[op_bgl_depth];
    ++op_bgl_current;
    ++op_bgl_depth;
    memcpy(op_bgl_current, previous, sizeof(*previous));
}
