#include "api.h"
// FUNCTION: WMAIN 0x0046b1f4
void op_font_get_colours(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    if (!op_current_font)
        return;
    *a = op_current_font->colours[0];
    *b = op_current_font->colours[1];
    *c = op_current_font->colours[2];
    *d = op_current_font->colours[3];
}
