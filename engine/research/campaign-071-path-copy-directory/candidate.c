#include "api.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046f870
void op_path_copy_directory(const char *source, char *destination)
{
    int start, end;
    destination[0] = 0;
    start = op_path_first_directory_offset(source);
    end = op_path_file_offset(source);
    if (start != -1) {
        if (end == -1)
            end = strlen(source);
        op_copy_keyframe_name(destination, source + start, end - start);
        *(destination + end - start) = 0;
    }
}
