#include "utilities.h"
// FUNCTION: WMAIN 0x0049746e
int op_util_file_exists(const char *path)
{
    int handle;
    handle = op_conf_services->open(path, op_conf_read_mode);
    if (!handle)
        return 0;
    op_conf_services->close(handle);
    return 1;
}
