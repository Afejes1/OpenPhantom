#include "api.h"
// FUNCTION: WMAIN 0x00451fb0
int op_save_read_raw(void *data, unsigned int bytes)
{
    return op_save_file_read(data, bytes, 1, op_save_input_stream);
}
