#include "api.h"
// FUNCTION: WMAIN 0x004392cd
void op_effects_restore(void)
{
    if (op_effects_save.enabled)
    {
        op_renderer_set_flags(op_renderer_get_flags() & ~0x40u);
        op_renderer_set_fog_start(op_effects_save.start);
        op_renderer_set_fog_end(op_effects_save.end);
        op_renderer_set_fog_rgb(op_effects_save.red, op_effects_save.green, op_effects_save.blue);
        op_renderer_set_clear_rgb(0, 0, 0);
        op_renderer_set_flags(op_renderer_get_flags() | 0x40u);
        op_active_world->flags |= 1;
    }
    op_shield_relink();
    op_overlay_restore_state();
}
