#include "api.h"
// FUNCTION: WMAIN 0x004612a1
void op_ui_clear_joy_latches(void)
{
    op_ui_joy_latches.first = 0;
    op_ui_joy_latches.second = 0;
}
