#include "conf_stream.h"
// FUNCTION: WMAIN 0x0048bdad
int op_conf_read_bytes(void *data, unsigned int count)
{
    if (!op_conf_read_active || !op_conf_read_handle)
        return 0;
    if (op_conf_services->read(op_conf_read_handle, data, count) != count)
        return 0;
    return 1;
}
