#include "culling.h"
#include <string.h>
/* FUNCTION: WMAIN 0x00403FA0 */
int op_cull_scan_plane(int plane, const float *position, float height)
{
    float base[3];
    op_scan_vector corner;
    op_scan_vector projected;
    int index = 0;
    int *pair;
    memcpy(base, position, sizeof(base));
    if (height != 0.0f) base[0] += height * op_scan_step_z[0];
    base[1] += height * op_scan_step_z[1];
    base[2] += height * op_scan_step_z[2];
    pair = &op_scan_corner_pairs[plane][1];
    for (; index < 4; index += 2, pair += 2) {
        memcpy(&corner, base, sizeof(corner));
        if (pair[-1] != 0) {
            corner.x += op_scan_step_x[0];
            corner.y += op_scan_step_x[1];
            corner.z += op_scan_step_x[2];
        }
        if (pair[0] != 0) {
            corner.x += op_scan_step_y[0];
            corner.y += op_scan_step_y[1];
            corner.z += op_scan_step_y[2];
        }
        if (corner.y <= 0.0f) return 1;
        ((op_scan_camera *)op_camera)->project(&projected, &corner);
        switch (plane) {
        case 0: if (projected.x <= op_scan_edge_20) return 0; break;
        case 1: if (projected.x >= op_scan_edge_18) return 0; break;
        case 2: if (projected.z >= 0.08f) return 0; break;
        case 3: if (projected.y >= op_scan_edge_1c) return 0; break;
        }
    }
    return 1;
}
