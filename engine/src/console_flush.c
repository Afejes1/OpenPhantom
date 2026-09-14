#include "console_helpers.h"
// FUNCTION: WMAIN 0x0048c640
void op_console_flush(void)
{
    op_console_flush_input(op_console_input.bits);
}
