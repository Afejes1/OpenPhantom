#include "api.h"
// FUNCTION: WMAIN 0x00498dbe
int op_kernel_set_callback(unsigned int callback)
{
    op_kernel_callback = callback;
    op_kernel_callback_enabled = callback != 0;
    return 1;
}
