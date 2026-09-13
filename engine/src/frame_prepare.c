#include "baseline.h"
// FUNCTION: WMAIN 0x0043f57a
void op_frame_prepare(void)
{
    op_prepare_world(op_active_world);
    op_traverse_world(op_active_world);
    op_prepare_world_surfaces(op_active_world, (unsigned char *)op_camera + 8, 0);
    op_prepare_graphics_submission();
    op_submit_gathered_surfaces();
}
