#include "conf_stream.h"
// FUNCTION: WMAIN 0x0048ba70
int op_conf_open_read(const char *path)
{
    return op_conf_open(path, op_conf_read_mode);
}
