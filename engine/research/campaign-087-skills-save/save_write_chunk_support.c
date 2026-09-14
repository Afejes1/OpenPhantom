#include "api.h"
// FUNCTION: WMAIN 0x00451c6d
int op_save_write_chunk(int context, const void *data, unsigned int bytes, unsigned short kind)
{
    if (op_save_header(context, bytes, kind) == 1)
        return 1;
    if (op_save_write(data, bytes))
        return 0;
    return 1;
}
