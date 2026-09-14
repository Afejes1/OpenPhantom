#include "api.h"
// FUNCTION: WMAIN 0x0048c539
void op_console_goto(short x, short y)
{
    OP_CONSOLE_COORD position;
    position.x = x;
    position.y = y;
    op_console_set_position(op_console_output.bits, position);
}
