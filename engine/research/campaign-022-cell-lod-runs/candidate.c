#include "api.h"
// FUNCTION: WMAIN 0x0041f56f
void op_cell_lod_runs(OP_CELL *cell)
{
    OP_RECORD *surface, *next_surface;
    int i, j, length;
    for (i = 0; i < cell->count; ++i)
    {
        length = 0;
        surface = &cell->records[i];
        if (surface->material_index == 65535)
            continue;
        if (surface->flags & 0x80)
            continue;
        for (j = i + 1; j < cell->count; ++j)
        {
            next_surface = &cell->records[j];
            if (next_surface->material_index == 65535)
                break;
            if ((surface->lod_mask & next_surface->lod_mask) == surface->lod_mask || (next_surface->flags & 0x80))
                ++length;
            else
                break;
            if (length >= 254)
                break;
        }
        surface->lod_run = (unsigned char)length;
    }
}
