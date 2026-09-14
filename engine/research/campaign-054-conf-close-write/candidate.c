#include "api.h"
// FUNCTION: WMAIN 0x0048bc49
void op_conf_close_write(void)
{
    if (op_conf_write_handle)
    {
        op_conf_services->close(op_conf_write_handle);
        op_conf_write_handle = 0;
        op_copy_keyframe_name(op_conf_write_name, op_conf_not_open, 127);
        op_conf_write_name[127] = 0;
    }
}
