#include "api.h"
// FUNCTION: WMAIN 0x0046ab6e
void op_debug_reset(unsigned int labels)
{
    int index;
    OP_DEBUG_ROW *row;
    for (index = 0, row = op_debug_rows; index < op_debug_count; index++, row++)
    {
        if (row->text)
            op_release(row->text);
        row->text = 0;
    }
    op_debug_count = 0;
    if (labels)
    {
        op_debug_upsert(0, 35, 24, op_debug_label0);
        op_debug_upsert(1, 35, 24, op_debug_label1);
        op_debug_upsert(2, 35, 24, op_debug_label2);
        op_debug_upsert(3, 35, 24, op_debug_label3);
    }
}
