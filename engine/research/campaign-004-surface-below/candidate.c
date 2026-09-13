#include "api.h"

// FUNCTION: WMAIN 0x0040c9f8
int op_has_surface_at_or_below(const float *position, unsigned int mask)
{
    op_query_point3 vertices[4];
    int index;
    op_query_surface *surface;
    unsigned int above_count;
    op_query_point3 normal;

    op_collect_nearby_surfaces(op_query_world, position, 0.0f);
    op_reset_surface_iterator();
    while ((surface = op_next_surface()) != 0) {
        if ((surface->runtime_flags & mask) != 0) {
            op_build_surface_geometry(op_query_world, surface, vertices, &normal);
            above_count = 0;
            for (index = 0; index < surface->vertex_count; ++index) {
                if (vertices[index].z > position[2])
                    ++above_count;
            }
            if (above_count == surface->vertex_count)
                continue;
            if (op_contains_surface_xy(position, surface, vertices) != 0)
                return 1;
        }
    }
    return 0;
}