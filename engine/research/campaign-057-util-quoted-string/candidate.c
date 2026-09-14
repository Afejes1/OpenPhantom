#include "api.h"
// FUNCTION: WMAIN 0x00497390
#pragma intrinsic(memcpy)
char *op_util_quoted_string(const char *input, char *out, unsigned int size)
{
    char *closing;
    unsigned int length;
    char *opening;
    if (out)
        *out = 0;
    opening = op_find_character(input, 34);
    if (!opening)
        return 0;
    ++opening;
    closing = op_find_character(opening, 34);
    if (!closing)
        return 0;
    if (out)
    {
        length = (unsigned int)(closing - opening);
        length = length < size - 1 ? length : size - 1;
        memcpy(out, opening, length);
        out[length] = 0;
    }
    return closing + 1;
}
