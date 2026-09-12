#include "baseline.h"

/* FUNCTION: WMAIN 0x00402046 */
int op_commit_projected_vertices(unsigned int requested)
{
    unsigned int color;
    float distance;
    unsigned int index;
    unsigned char alpha;
    float intensity;

    if (op_vertex_fog_enabled != 0) {
        for (index = op_projected_vertex_count;
             index < op_projected_vertex_count + requested; ++index) {
            color = 0xff000000u;
            if (op_projected_vertices[index].reciprocal_depth < op_fog_start_reciprocal) {
                color = 0;
                if (op_projected_vertices[index].reciprocal_depth >= op_fog_end_reciprocal) {
                    distance = (float)(1.0 / op_projected_vertices[index].reciprocal_depth - op_fog_start);
                    intensity = (float)((distance / op_fog_range) * 255.0);
                    alpha = (unsigned char)(255 - (int)intensity);
                    color = (unsigned int)alpha << 24;
                }
            }
            op_vertex_fog_alpha[index] = color;
        }
    }
    op_projected_vertex_count += requested;
    return 1;
}
