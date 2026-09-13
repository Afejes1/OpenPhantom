#ifndef OP_CAMPAIGN_004_SURFACE_BELOW_API_H
#define OP_CAMPAIGN_004_SURFACE_BELOW_API_H

typedef struct op_query_point3 { float x, y, z; } op_query_point3;
typedef struct op_query_surface {
    unsigned char opaque_00[0x25];
    unsigned char vertex_count;
    unsigned char opaque_26[0x16];
    unsigned short runtime_flags;
} op_query_surface;

extern void *op_query_world;
void op_collect_nearby_surfaces(void *world, const float *position, float radius);
void op_reset_surface_iterator(void);
op_query_surface *op_next_surface(void);
void op_build_surface_geometry(void *world, op_query_surface *surface,
                               op_query_point3 *vertices, op_query_point3 *normal);
int op_contains_surface_xy(const float *position, op_query_surface *surface,
                           const op_query_point3 *vertices);
int op_has_surface_at_or_below(const float *position, unsigned int mask);
#endif