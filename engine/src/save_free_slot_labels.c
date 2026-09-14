#include "save_slots.h"
// FUNCTION: WMAIN 0x00451efe
void op_save_free_slot_labels(OP_SAVE_LABEL_ROW *rows)
{
    int i;
    for (i = 0; i < 99; ++i)
    {
        if (rows[i].label)
        {
            op_release(rows[i].label);
            rows[i].label = 0;
        }
    }
}
