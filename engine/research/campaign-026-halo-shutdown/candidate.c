#include "api.h"
// FUNCTION: WMAIN 0x0043982b
void op_halo_shutdown(void)
{
    op_release_sprite(&op_halo_sprite_a);
    op_release_sprite(&op_halo_sprite_b);
    op_release_sprite(&op_halo_sprite_c);
}
