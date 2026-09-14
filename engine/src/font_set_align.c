#include "font_state.h"
// FUNCTION: WMAIN 0x0046b23c
void op_font_set_align(int align)
{
    if (!op_current_font)
        return;
    switch (align)
    {
    case 1:
        op_current_font->align = 2;
        break;
    case 2:
        op_current_font->align = 4;
        break;
    case 0:
        op_current_font->align = 1;
        break;
    }
}
