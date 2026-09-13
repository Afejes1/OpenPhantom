#include "halo_overlay.h"
// FUNCTION: WMAIN 0x00439a54
void op_halo_draw_actor(OP_ATTACHED_ACTOR *actor)
{
    int i;
    if (actor == 0)
        return;
    if ((actor->flags & 0x10) == 0)
        return;
    for (i = 0; i < 32; ++i)
        if (op_halos[i].owner == actor)
            op_halo_draw_slot(op_halos[i].owner, i);
}
