#include "api.h"
// FUNCTION: WMAIN 0x00451c58
int op_save_write(const void *data, unsigned int bytes)
{
    return op_save_write_raw(data, bytes);
}
