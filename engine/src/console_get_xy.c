#include "console_helpers.h"
// FUNCTION: WMAIN 0x0048c561
void op_console_get_xy(OP_CONSOLE_COORD *output)
{
    OP_CONSOLE_INFO info;
    op_console_get_info(op_console_output.bits, &info);
    output->x = info.cursor.x;
    output->y = info.cursor.y;
}
