#include "console_control.h"
// FUNCTION: WMAIN 0x0048c6ef
void op_console_put_char(char value, unsigned int attr)
{
    unsigned int written;
    if (op_console_current_attr.low != (attr & 0xffffu))
        op_console_set_attr((unsigned short)attr);
    op_console_write(op_console_output.bits, &value, 1, &written, 0);
}
