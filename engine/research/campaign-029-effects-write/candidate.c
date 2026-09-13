#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0043912f
int op_effects_write(int value)
{
    int size = 0;
    size += 96;
    memset(&op_effects_save, 0, 96);
    op_effects_save.duration = op_fog_duration;
    op_effects_save.target = op_fog_target;
    op_effects_save.cached = op_fog_cached_start;
    op_effects_save.remaining = op_fog_remaining;
    op_effects_save.restore = op_fog_restore_mode;
    op_effects_save.saved = op_fog_saved;
    op_effects_save.enabled = op_active_world->flags & 1;
    op_get_fog_rgb(&op_effects_save.red, &op_effects_save.green, &op_effects_save.blue);
    op_get_fog_range(&op_effects_save.start, &op_effects_save.end);
    op_effects_save.shield_size = op_shield_save_size();
    size += op_effects_save.shield_size;
    op_effects_save.overlay_size = op_overlay_save_size();
    size += op_effects_save.overlay_size;
    op_save_header(value, size, 0x103);
    op_save_write(&op_effects_save, 96);
    op_shield_write();
    op_overlay_save_state();
    return 0;
}
