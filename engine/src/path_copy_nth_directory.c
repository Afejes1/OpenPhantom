#include "path_helpers.h"
// FUNCTION: WMAIN 0x0046f680
void op_path_copy_nth_directory(const char *source, int index, char *destination)
{
    const char *cursor;
    int offset;
    destination[0] = 0;
    offset = op_path_directory_offset(source, index);
    if (offset != -1)
    {
        cursor = source + offset;
        offset = op_path_separator_offset(cursor);
        if (offset > 0)
        {
            op_copy_keyframe_name(destination, cursor, offset);
            destination[offset] = 0;
        }
    }
}
