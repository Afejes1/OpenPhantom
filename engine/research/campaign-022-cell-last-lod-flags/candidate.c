#include "api.h"
// FUNCTION: WMAIN 0x0041f37e
void op_cell_last_lod_flags(OP_CELL *cell)
{
    OP_RECORD *surface, *next_surface;
    int i, j, count, is_last;
    for (i = 0; i < cell->count; ++i)
    {
        is_last = 1;
        count = 0;
        surface = &cell->records[i];
        if (surface->material_index == 65535)
            continue;
        for (j = i + 1; j < cell->count; ++j)
        {
            next_surface = &cell->records[j];
            if (next_surface->material_index == 65535 ||
                (surface->lod_mask & next_surface->lod_mask) == surface->lod_mask)
            {
                is_last = 0;
                break;
            }
            ++count;
        }
        if (is_last)
            surface->last_lod_flags |= 8;
        else
            surface->last_lod_flags &= ~8;
    }
}
