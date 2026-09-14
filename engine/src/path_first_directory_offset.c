#include "path_helpers.h"
// FUNCTION: WMAIN 0x0046fa53
int op_path_first_directory_offset(const char *path)
{
    const char *cursor = path;
    while (*cursor) {
        if (*cursor == '\\') {
            ++cursor;
            continue;
        }
        if (cursor[1] == ':') {
            cursor += 2;
            continue;
        }
        if (op_path_separator_offset(cursor) == -1)
            break;
        return cursor - path;
    }
    return -1;
}
