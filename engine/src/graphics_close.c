#include "application_helpers.h"
// FUNCTION: WMAIN 0x0043f5c0
void op_graphics_close(void)
{
    op_clear_graphics_state_a();
    op_clear_graphics_state_b();
    op_close_graphics_device();
    op_shutdown_graphics();
}
