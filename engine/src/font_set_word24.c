#include "font_state.h"
// FUNCTION: WMAIN 0x0046b75e
int op_font_set_word24(unsigned int word)
{
    if (!op_current_font)
        return 0;
    op_current_font->word24 = word;
    return 1;
}
