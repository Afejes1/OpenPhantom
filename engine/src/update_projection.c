#include "baseline.h"

// FUNCTION: WMAIN 0x0040ef40
/* Candidate: publish camera/viewport/frustum values into projection globals.
 * The integer edge fields are converted to float; width/height are bit copies.
 */
void op_update_projection(void)
{
    op_width_bits = op_camera->viewport->width_bits;
    op_height_bits = op_camera->viewport->height_bits;
    op_focal = op_camera->focal;
    op_depth_scale = 1.0f / op_camera->frustum->far_bound;
    op_edge_18 = (float)op_camera->viewport->edge_18;
    op_edge_1c = (float)op_camera->viewport->edge_1c;
    op_edge_20 = (float)op_camera->viewport->edge_20;
    op_edge_24 = (float)op_camera->viewport->edge_24;
    op_far = op_camera->frustum->far_bound;
    op_near = op_camera->frustum->near_bound;
}
