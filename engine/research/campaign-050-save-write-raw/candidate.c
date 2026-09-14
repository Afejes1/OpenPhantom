#include "api.h"
// FUNCTION: WMAIN 0x00451f4e
int op_save_write_raw(const void *data, unsigned int bytes)
{
    int result;
    result = op_stream_write(data, bytes, 1, op_save_output_stream);
    if (!result)
    {
        op_save_error_begin(op_save_error_context);
        op_save_error_notify(17, -1, 0, op_save_failure_text, 3840);
        op_save_error_end(op_save_error_context);
        return 0;
    }
    return result;
}
