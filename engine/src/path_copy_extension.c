#include "path_helpers.h"
#pragma intrinsic(strcat)
// FUNCTION: WMAIN 0x0046f747
void op_path_copy_extension(const char *source, char *destination)
{
    const char *cursor;
    int offset;
    destination[0] = 0;
    offset = op_path_extension_offset(source);
    if (offset != -1) {
        cursor = source + offset;
        strcat(destination, cursor);
    }
}
