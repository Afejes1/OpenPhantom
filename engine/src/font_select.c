#include "font_state.h"
// FUNCTION: WMAIN 0x0046b13b
void op_font_select(int index)
{
    if (index < 0 || index >= 16 || !op_font_pool.rows[index].font)
        op_current_font = 0;
    else
        op_current_font = &op_font_pool.rows[index];
}
