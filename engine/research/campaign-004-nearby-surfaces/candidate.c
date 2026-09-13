#include "api.h"
#include <string.h>
#pragma intrinsic(memset)

/* FUNCTION: WMAIN 0x0040D03E, collect nearby collision surfaces. */
void op_collect_nearby_surfaces(void *world, const float *position, float radius)
{
    int cellCount;
    int added = 0;
    op_query_cell cells[25];
    int total = 0;
    int i;

    (void)world;
    if (radius >= 2.0f)
        op_query_services->assert_failed(op_query_radius_assert,
                                         op_query_assert_file, 0x36e);
    if (radius < 0.5f)
        cellCount = op_query_small_cells(cells, position, radius);
    else
        cellCount = op_query_square_cells(cells, position, radius, 25);

    if (op_query_cached_generation == op_query_generation &&
        cellCount == op_query_cached_cell_count) {
        for (i = 0; i < cellCount; ++i) {
            if (cells[i].x != op_query_cached_cells[i].x ||
                cells[i].y != op_query_cached_cells[i].y)
                break;
            return;
        }
    }

    memset(op_query_visited, 0, 255);
    for (i = 0; i < cellCount; ++i) {
        added = op_query_append_cell(op_query_active_world, cells[i].x,
                                     cells[i].y, total, 0);
        op_query_cached_cells[i].x = cells[i].x;
        op_query_cached_cells[i].y = cells[i].y;
        op_query_cached_cells[i].count = added;
        total += added;
    }
    if (total >= 4096)
        op_query_services->assert_failed(op_query_capacity_assert,
                                         op_query_assert_file, 0x395);
    op_query_polygon_count = total;
    op_query_cached_cell_count = cellCount;
    op_query_cached_generation = op_query_generation;
}