#include "projected_overlay.h"
#include <string.h>
// FUNCTION: WMAIN 0x0041C970
void op_projection_sort(void)
{
    if (op_projection_dirty)
        op_projection_qsort(op_projections, 255, sizeof(OP_PROJECTION), op_projection_compare);
    op_projection_dirty = 0;
}
