#include "api.h"
// FUNCTION: WMAIN 0x0041f2ce
void op_cell_precompute(OP_CELL *cell)
{
    if (cell != 0)
    {
        op_cell_content_flags(cell);
        op_cell_lod_mask(cell);
        op_cell_material_runs(cell);
        op_cell_lod_runs(cell);
        op_cell_last_lod_flags(cell);
    }
}
