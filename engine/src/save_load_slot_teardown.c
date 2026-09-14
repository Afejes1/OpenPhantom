#include "save_slots.h"
// FUNCTION: WMAIN 0x00451936
int op_save_load_slot_teardown(int slot)
{
    op_save_lifecycle(0, 6);
    return op_load_saved_game(op_save_slot_path(slot), 0);
}
