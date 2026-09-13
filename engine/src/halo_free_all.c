#include "halo_overlay.h"
// FUNCTION: WMAIN 0x0043a1bb
void op_halo_free_all(void)
{
    int i;
    for (i = 0; i < 32; ++i)
        if (op_halos[i].owner != 0)
        {
            op_halos[i].owner = 0;
            op_release_sprite(&op_halos[i].sprite);
            --op_halo_count;
        }
    op_halo_count = 0;
}
