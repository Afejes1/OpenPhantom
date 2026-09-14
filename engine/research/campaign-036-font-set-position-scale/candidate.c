#include "api.h"
// FUNCTION: WMAIN 0x0046b2ba
void op_font_set_position_scale(float x, float y)
{
    if (!op_current_font)
        return;
    op_current_font->position_x = x;
    op_current_font->position_y = y;
}
