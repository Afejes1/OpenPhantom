#include "font_state.h"
// FUNCTION: WMAIN 0x0046b2fc
int op_font_measure_char(char character, float *width, float *height)
{
    int result;
    float w, h;
    if (!op_current_font)
        return 0;
    op_set_font_scale(op_current_font->glyph_x, op_current_font->glyph_y);
    result = op_font_char_size(op_current_font->font, character, &w, &h);
    if (!result)
        return 0;
    if (width)
        *width = w;
    if (height)
        *height = h;
    return 1;
}
