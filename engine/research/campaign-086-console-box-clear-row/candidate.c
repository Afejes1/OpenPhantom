#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0048c7c8
void op_console_box_clear_row(OP_CONSOLE_BOX *box, int row)
{
    memset((unsigned char *)box->buffer + row * (box->width - 2), ' ', box->width - 2);
}
