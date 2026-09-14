#include "api.h"
// FUNCTION: WMAIN 0x0048bced
int op_conf_write_bytes(const void *data, unsigned int count)
{
    unsigned int result;
    if (!op_conf_write_handle || !data)
        return 0;
    result = op_conf_services->write(op_conf_write_handle, data, count);
    if (count != result)
        return 0;
    return 1;
}
