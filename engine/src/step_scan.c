#include "baseline.h"
/* Explicit promotions on coordinates 1 and 2 preserve VC5 x87 scheduling.
 * This is a matching source recipe, not a claim about historical types.
 */
/* FUNCTION: WMAIN 0x00403EF0 */
void op_step_scan(float *position, int x_step, int y_step)
{
    if (x_step == 1) {
        position[0] += op_scan_step_x[0];
        position[1] = (float)((double)position[1] + op_scan_step_x[1]);
        position[2] = (float)((double)position[2] + op_scan_step_x[2]);
    } else if (x_step == -1) {
        position[0] -= op_scan_step_x[0];
        position[1] -= op_scan_step_x[1];
        position[2] -= op_scan_step_x[2];
    }
    if (y_step == 1) {
        position[0] += op_scan_step_y[0];
        position[1] = (float)((double)position[1] + op_scan_step_y[1]);
        position[2] = (float)((double)position[2] + op_scan_step_y[2]);
    } else if (y_step == -1) {
        position[0] -= op_scan_step_y[0];
        position[1] -= op_scan_step_y[1];
        position[2] -= op_scan_step_y[2];
    }
}
