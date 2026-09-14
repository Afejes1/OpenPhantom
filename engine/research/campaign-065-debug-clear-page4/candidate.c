#include "api.h"
// FUNCTION: WMAIN 0x0046a7ac
void op_debug_clear_page4(void)
{
    int index;
    OP_DEBUG_ROW *row;
    for (index = 0, row = op_debug_rows; index < op_debug_count; index++, row++)
    {
        if (row->page == 4 && row->text)
        {
            op_release(row->text);
            row->text = 0;
        }
    }
}
