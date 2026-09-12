#include "baseline.h"

// FUNCTION: WMAIN 0x0040ef40
/* VC5-calibrated: publish camera/viewport/frustum values into projection globals.
 * The integer edge fields are converted to float; width/height are bit copies.
 */
void op_update_projection(void)
{
    float far_value;
    op_width_bits = op_camera->viewport->width_bits;
    op_height_bits = op_camera->viewport->height_bits;
    op_focal = op_camera->focal;
    /* The local preserves VC5's load-then-reciprocal evaluation order. */
    far_value = op_camera->frustum->far_bound;
    op_depth_scale = 1.0f / far_value;
    op_edge_18 = (float)op_camera->viewport->edge_18;
    op_edge_1c = (float)op_camera->viewport->edge_1c;
    op_edge_20 = (float)op_camera->viewport->edge_20;
    op_edge_24 = (float)op_camera->viewport->edge_24;
    op_far = op_camera->frustum->far_bound;
    op_near = op_camera->frustum->near_bound;
}
