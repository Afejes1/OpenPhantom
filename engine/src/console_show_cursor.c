#include "console_control.h"
// FUNCTION: WMAIN 0x0048c591
void op_console_show_cursor(int show)
{
    if (show)
    {
        if ((int)op_console_cursor_depth > 0)
        {
            --op_console_cursor_depth;
            if (!op_console_cursor_depth)
            {
                op_console_cursor_info.visible = 1;
                op_console_set_cursor_info(op_console_output.bits, &op_console_cursor_info);
            }
        }
    }
    else
    {
        if (!op_console_cursor_depth)
        {
            op_console_cursor_info.visible = 0;
            op_console_set_cursor_info(op_console_output.bits, &op_console_cursor_info);
        }
        ++op_console_cursor_depth;
    }
}
