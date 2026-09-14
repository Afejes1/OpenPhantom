#include "console_control.h"
#include <string.h>
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0048c732
void op_console_put_string(const char *text, unsigned int attr)
{
    unsigned int written;
    if (op_console_current_attr.low != (attr & 0xffffu))
        op_console_set_attr((unsigned short)attr);
    op_console_write(op_console_output.bits, text, strlen(text), &written, 0);
}
