#include "api.h"
// FUNCTION: WMAIN 0x0041f770
void op_bgl_pop(void)
{
    if (op_bgl_depth <= 0)
        return;
    --op_bgl_current;
    --op_bgl_depth;
}
