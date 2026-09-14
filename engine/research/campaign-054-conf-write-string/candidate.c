#include "api.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0048bc8e
int op_conf_write_string(const char *text)
{
    unsigned int length, result;
    if (!op_conf_write_handle || !text)
        return 0;
    length = strlen(text);
    result = op_conf_services->write(op_conf_write_handle, text, length);
    if (length != result)
        return 0;
    return 1;
}
