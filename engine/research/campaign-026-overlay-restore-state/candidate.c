#include "api.h"
// FUNCTION: WMAIN 0x004397b5
int op_overlay_restore_state(void)
{
    op_letterbox_visible = op_overlay_save.visible;
    op_letterbox_target = op_overlay_save.target;
    op_letterbox_previous = op_overlay_save.previous;
    op_letterbox_step = op_overlay_save.step;
    return 0;
}
