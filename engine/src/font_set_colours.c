#include "font_state.h"
// FUNCTION: WMAIN 0x0046b1b6
void op_font_set_colours(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    if (!op_current_font)
        return;
    op_current_font->colours[0] = a;
    op_current_font->colours[1] = b;
    op_current_font->colours[2] = c;
    op_current_font->colours[3] = d;
}
