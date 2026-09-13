#include "api.h"
// FUNCTION: WMAIN 0x00438e78
void op_effects_draw_object(OP_ACTOR *actor, float *transform, int unused, int draw_halo)
{
    (void)unused;
    if (draw_halo != 0)
        op_halo_draw_actor(actor);
    if ((actor->flags & 1) != 0 && actor->shield_slot >= 0)
        op_shield_draw_attached(actor->shield_slot, transform);
}
