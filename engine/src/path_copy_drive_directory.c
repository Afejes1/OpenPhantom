#include "path_helpers.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046f8e4
void op_path_copy_drive_directory(const char *source, char *destination)
{
    int result, end, start;
    char drive;
    destination[0] = 0;
    drive = op_path_drive(source);
    if (drive) {
        destination[0] = drive;
        destination[1] = ':';
        destination[2] = 92;
        destination[3] = 0;
        result = 3;
    } else {
        destination[0] = 0;
        result = 0;
    }
    start = op_path_first_directory_offset(source);
    end = op_path_file_offset(source);
    if (start != -1) {
        if (end == -1)
            end = strlen(source);
        op_copy_keyframe_name(destination + result, source + start, end - start);
        *(destination + result + end - start) = 0;
    }
}
