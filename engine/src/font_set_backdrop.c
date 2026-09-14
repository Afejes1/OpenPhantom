#include "font_state.h"
// FUNCTION: WMAIN 0x0046b5f0
int op_font_set_backdrop(char *name, float width, float height)
{
    if (!op_current_font)
        return 0;
    op_current_font->backdrop_width = width;
    op_current_font->backdrop_height = height;
    op_release_sprite(&op_current_font->backdrop);
    if (!name)
        return 1;
    op_current_font->backdrop = op_acquire_sprite(name);
    if (!op_current_font->backdrop)
        return 0;
    return 1;
}
