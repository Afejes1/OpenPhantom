#include "api.h"
// FUNCTION: WMAIN 0x0046b37a
float op_font_measure_string(char *text)
{
    float width, total;
    total = 0.0f;
    while (*text)
    {
        op_font_measure_char(*text, &width, 0);
        total += width;
        ++text;
    }
    return total;
}
