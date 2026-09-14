#include "api.h"
// FUNCTION: WMAIN 0x0046b662
int op_font_set_zrange(float start, float range)
{
    if (!op_current_font)
        return 0;
    op_current_font->z_start = start;
    op_current_font->z_end = start + range;
    return 1;
}
