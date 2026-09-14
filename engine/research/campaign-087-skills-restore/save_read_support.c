#include "api.h"
// FUNCTION: WMAIN 0x00451cb0
int op_save_read(void *data, unsigned int bytes)
{
    return op_save_read_raw(data, bytes);
}
