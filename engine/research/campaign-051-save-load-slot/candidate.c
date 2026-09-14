#include "api.h"
// FUNCTION: WMAIN 0x0045195e
void op_save_load_slot(int slot)
{
    op_load_saved_game(op_save_slot_path(slot), 1);
}
