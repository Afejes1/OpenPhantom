#include "api.h"
// FUNCTION: WMAIN 0x0048c651
void op_console_clear(void)
{
    OP_CONSOLE_COORD origin;
    unsigned int written_count;
    op_console_set_attr((unsigned short)(op_console_default_attr.low & 0xf0));
    origin.x = 0;
    origin.y = 0;
    op_console_fill_character(op_console_output.bits, ' ', 2000, origin, &written_count);
    op_console_set_attr(op_console_default_attr.low);
}
