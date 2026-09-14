#include "console_control.h"
// FUNCTION: WMAIN 0x0048c519
void op_console_box_destroy(OP_CONSOLE_BOX *box)
{
    op_platform_heap_free(box->buffer);
    op_platform_heap_free(box);
}
