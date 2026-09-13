#include "api.h"
// FUNCTION: WMAIN 0x004397f0
void op_halo_startup(void)
{
    op_halo_sprite_a = op_acquire_sprite(op_halo_name_a);
    op_halo_sprite_b = op_acquire_sprite(op_halo_name_b);
    op_halo_sprite_c = op_acquire_sprite(op_halo_name_c);
}
