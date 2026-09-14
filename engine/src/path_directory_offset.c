#include "path_helpers.h"
// FUNCTION: WMAIN 0x0046f9a8
int op_path_directory_offset(const char *source, int index)
{
    const char *cursor;
    int offset;
    offset = op_path_first_directory_offset(source);
    if (offset == -1)
        goto missing;
    cursor = source + offset;
    while (--index != 0)
    {
        for (;;)
        {
            if (!*cursor)
                goto missing;
            if (*cursor == 92)
                break;
            ++cursor;
        }
        ++cursor;
        while (*cursor == 92)
            ++cursor;
        if (!*cursor)
            goto missing;
    }
    if (op_path_separator_offset(cursor) == -1)
        goto missing;
    return cursor - source;
missing:
    return -1;
}
