#include "api.h"
// FUNCTION: WMAIN 0x0041f315
void op_cell_lod_mask(OP_CELL *cell)
{
    unsigned char lodMask = 255;
    int i;
    for (i = 0; i < cell->count; ++i)
    {
        OP_RECORD *pSurface;
        pSurface = &cell->records[i];
        if (pSurface->material_index == 65535)
        {
            lodMask = 0;
            break;
        }
        lodMask &= pSurface->lod_mask;
    }
    cell->lod_mask = lodMask;
}
