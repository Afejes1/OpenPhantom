#include "api.h"
// FUNCTION: WMAIN 0x00438f37
void op_fog_restore(void)
{
    op_active_world->flags &= ~1U;
    op_active_world->flags |= op_fog_saved;
    op_world_apply_fog(op_active_world);
}
