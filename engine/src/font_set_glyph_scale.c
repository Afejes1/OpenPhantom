#include "font_state.h"
// FUNCTION: WMAIN 0x0046b293
void op_font_set_glyph_scale(float x, float y)
{
    if (!op_current_font)
        return;
    op_current_font->glyph_x = x;
    op_current_font->glyph_y = y;
}
