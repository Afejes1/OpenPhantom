#include "api.h"
// FUNCTION: WMAIN 0x0046b179
void op_font_set_colour(unsigned int colour)
{
    int i;
    if (!op_current_font)
        return;
    for (i = 0; i < 4; ++i)
        op_current_font->colours[i] = colour;
}
