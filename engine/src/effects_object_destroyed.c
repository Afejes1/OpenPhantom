#include "focused_accessors.h"
// FUNCTION: WMAIN 0x00438ee5
void op_effects_object_destroyed(OP_ATTACHED_ACTOR *object)
{
    op_detach_halo(object);
    op_shield_stop(object->shield_slot);
    op_detach_zap(object);
}
