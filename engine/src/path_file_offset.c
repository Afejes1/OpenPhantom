#include "path_helpers.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046fab6
int op_path_file_offset(const char *path)
{
    const char *cursor;
    int length;
    length = strlen(path);
    if (length) {
        cursor = path + length - 1;
        while (cursor >= path) {
            if (*cursor == '\\')
                break;
            if (*cursor == ':')
                break;
            --cursor;
        }
        ++cursor;
        if (*cursor)
            return cursor - path;
    }
    return -1;
}
