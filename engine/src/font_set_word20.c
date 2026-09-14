#include "font_state.h"
// FUNCTION: WMAIN 0x0046b2e1
void op_font_set_word20(unsigned int word)
{
    if (!op_current_font)
        return;
    op_current_font->word20 = word;
}
