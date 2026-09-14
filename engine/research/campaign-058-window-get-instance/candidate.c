#include "api.h"
// FUNCTION: WMAIN 0x0049897e
OP_WINDOW_HANDLE_WORD op_window_get_instance(void)
{
    return op_instance_handle;
}
