#include "api.h"
// FUNCTION: WMAIN 0x00497440
void op_util_strlwr(char *text)
{
    while (*text)
    {
        *text = (char)op_tolower(*text);
        ++text;
    }
}
