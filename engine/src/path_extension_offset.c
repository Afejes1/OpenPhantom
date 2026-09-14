#include "path_helpers.h"
// FUNCTION: WMAIN 0x0046fb32
int op_path_extension_offset(const char *path)
{
    const char *cursor;
    int offset;
    offset = op_path_file_offset(path);
    if (offset != -1) {
        cursor = path + offset;
        while (*cursor) {
            if (*cursor == '.') {
                ++cursor;
                if (*cursor)
                    return cursor - path;
            }
            ++cursor;
        }
    }
    return -1;
}
