#include "bgl_state.h"
// FUNCTION: WMAIN 0x0041f700
void op_bgl_identity(void)
{
    *op_bgl_current = op_bgl_identity_matrix;
}
