#include "baseline.h"

/* FUNCTION: WMAIN 0x00406830 */
void op_submit_gathered_surfaces(void)
{
    if (op_gathered_surface_count != 0) {
        op_last_submitted_surface_count = op_draw_surface_buckets(
            op_gathered_world, (unsigned char *)op_camera + 8,
            op_gathered_surface_count);
    }
}
