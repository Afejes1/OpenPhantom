#include "api.h"
// FUNCTION: WMAIN 0x00439752
int op_overlay_save_state(void)
{
    op_overlay_save.visible = op_letterbox_visible;
    op_overlay_save.target = op_letterbox_target;
    op_overlay_save.previous = op_letterbox_previous;
    op_overlay_save.step = op_letterbox_step;
    op_save_write(&op_overlay_save, 28);
    return 0;
}
