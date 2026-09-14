#include "path_helpers.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046f7aa
void op_path_copy_stem(const char *source, char *destination)
{
    /* end is the directory-prefix end; start is the extension start. */
    const char *result;
    int end, start;
    destination[0] = 0;
    end = op_path_file_offset(source);
    start = op_path_extension_offset(source);
    if (end != -1) {
        result = source + end;
        if (start == -1)
            start = strlen(source);
        else
            --start;
        op_copy_keyframe_name(destination, result, start - end);
        *(destination + start - end) = 0;
    }
}
