#include "api.h"
// FUNCTION: WMAIN 0x0041f230
int op_world_precompute_grid(OP_WORLD *world)
{
    int x, y;
    OP_CELL *cell;
    if (world->cells == 0)
        return 0;
    for (y = 0; y < world->height; ++y)
    {
        for (x = 0; x < world->width; ++x)
        {
            cell = (world->cells + x)[y * world->width];
            if (cell == 0 || cell->count <= 0)
                continue;
            op_cell_precompute(cell);
        }
    }
    return 1;
}
