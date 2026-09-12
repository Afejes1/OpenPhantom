#include "baseline.h"

/* FUNCTION: WMAIN 0x00402130 */
op_projected_vertex *op_peek_projected_vertices(unsigned int requested)
{
    if (op_projected_vertex_count + requested > 8192u)
        return 0;
    return &op_projected_vertices[op_projected_vertex_count];
}
