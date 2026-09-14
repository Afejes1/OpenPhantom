#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0048c785
void op_console_box_clear(OP_CONSOLE_BOX *box)
{
    memset(box->buffer, ' ', box->inner_size);
    box->column = 0;
    box->row = 0;
}
