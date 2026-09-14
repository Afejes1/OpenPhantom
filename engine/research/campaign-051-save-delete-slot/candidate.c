#include "api.h"
// FUNCTION: WMAIN 0x004519a0
int op_save_delete_slot(int slot)
{
    if (op_delete_file(op_save_slot_path(slot)) == 0)
        return 0;
    return 1;
}
