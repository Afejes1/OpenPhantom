#include "api.h"
// FUNCTION: WMAIN 0x0043a154
void op_detach_halo(OP_ACTOR *actor)
{
    int i;
    for (i = 0; i < 32; ++i)
    {
        if (op_halos[i].owner == actor)
        {
            op_halos[i].owner = 0;
            op_release_sprite(&op_halos[i].sprite);
            --op_halo_count;
        }
    }
}
