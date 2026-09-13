#include "halo_overlay.h"
// FUNCTION: WMAIN 0x00438ebd
void op_effects_object_visibility(OP_ATTACHED_ACTOR *actor)
{
    op_model_ambient_boost = 0;
    if ((actor->flags & 0x100) != 0)
        op_model_ambient_boost = 1;
}
