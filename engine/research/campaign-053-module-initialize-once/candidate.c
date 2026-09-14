#include "api.h"
// FUNCTION: WMAIN 0x0046ebe0
int op_module_initialize_once(void)
{
    int result = 1;
    if (!op_module_initialized)
    {
        op_module_initialized = 1;
        result = 0;
    }
    return result;
}
