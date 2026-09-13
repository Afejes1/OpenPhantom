#include "halo_overlay.h"
// FUNCTION: WMAIN 0x00438e67
void op_effects_object_created(OP_ATTACHED_ACTOR *actor)
{
    op_halo_attach(actor);
}
