#include "api.h"
// FUNCTION: WMAIN 0x00439235
int op_effects_load(int kind)
{
    if (kind != 0x103)
        return 1;
    if (op_save_read(&op_effects_save, 96) == 0)
        return 1;
    op_fog_duration = op_effects_save.duration;
    op_fog_target = op_effects_save.target;
    op_fog_cached_start = op_effects_save.cached;
    op_fog_remaining = op_effects_save.remaining;
    op_fog_restore_mode = op_effects_save.restore;
    op_fog_saved = op_effects_save.saved;
    if (op_shield_load() != 0)
        return 1;
    if (op_overlay_read_state() != 0)
        return 1;
    return 0;
}
