#include "effects_state.h"
// FUNCTION: WMAIN 0x00438f14
void op_fog_snapshot(void)
{
    op_fog_remaining = -1.0f;
    op_fog_saved = op_active_world->flags & 1;
}
