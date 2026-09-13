#include "api.h"
// FUNCTION: WMAIN 0x0041f477
void op_cell_material_runs(OP_CELL *cell)
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
            if (surface->material_group == next_surface->material_group || (next_surface->flags & 0x80))
                ++length;
            else
                break;
            if (length >= 254)
                break;
        }
        surface->run_length = (unsigned char)length;
    }
}
