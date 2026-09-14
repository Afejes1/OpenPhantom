#include "api.h"
// FUNCTION: WMAIN 0x0046fb98
int op_path_separator_offset(const char *path)
{
    const char *found;
    found = op_find_character(path, '\\');
    if (!found)
        return -1;
    return found - path;
}
