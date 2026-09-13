#ifndef OP_CAMPAIGN_003_POLYLINE_API_H
#define OP_CAMPAIGN_003_POLYLINE_API_H

#include "baseline.h"

typedef struct op_polyline_vertex {
    float x, y, z, rhw;
    unsigned int color, specular;
    float u, v;
} op_polyline_vertex;

#ifdef __cplusplus
extern "C" {
#endif

int op_submit_polyline_quad(void *material, unsigned int flags,
                            const op_polyline_vertex *vertices,
                            int vertex_count, int mode);
void op_submit_polyline(const float *points, int count, unsigned int color,
                        void *material, float width);

#ifdef __cplusplus
}
#endif

#endif
