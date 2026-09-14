#include "api.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046f82f
char op_path_drive(const char *path)
{
    int length;
    length = strlen(path);
    if (length > 1 && path[1] == ':')
        return (char)op_toupper(path[0]);
    return 0;
}
