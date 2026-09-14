#include "save_slots.h"
// FUNCTION: WMAIN 0x00451fe9
char *op_save_slot_path(int slot)
{
    op_format_sprite_name(op_save_path, op_save_path_format, op_save_path_prefix, slot);
    return op_save_path;
}
