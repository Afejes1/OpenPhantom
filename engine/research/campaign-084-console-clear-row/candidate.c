#include "api.h"
// FUNCTION: WMAIN 0x0048c6a8
void op_console_clear_row(short row)
{
    OP_CONSOLE_COORD origin;
    unsigned int written_count;
    op_console_set_attr((unsigned short)(op_console_default_attr.low & 0xf0));
    origin.x = 0;
    origin.y = row;
    op_console_fill_character(op_console_output.bits, ' ', 80, origin, &written_count);
}
