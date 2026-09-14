#include "api.h"
// FUNCTION: WMAIN 0x00497250
#pragma intrinsic(strlen, strcpy)
char *op_util_strdup(const char *source)
{
    char *copy;
    copy = (char *)op_allocate(strlen(source) + 1);
    if (!copy)
        return 0;
    strcpy(copy, source);
    return copy;
}
