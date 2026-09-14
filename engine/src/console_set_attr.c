#include "console_helpers.h"
// FUNCTION: WMAIN 0x0048c61f
void op_console_set_attr(unsigned short attr)
{
    op_console_current_attr.low = attr;
    op_console_set_text_attribute(op_console_output.bits, attr);
}
