#include "ui_widget_control.h"
// FUNCTION: WMAIN 0x0045c214
void op_ui3d_end(void)
{
    if (op_ui3d_active == 1)
    {
        op_ui3d_flush_material_buckets();
        op_ui3d_flush_draw_queue();
        op_ui3d_finish_backend();
        op_ui3d_active = 0;
    }
}
