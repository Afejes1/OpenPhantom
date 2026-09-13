#ifndef OP_FLUSH_RESEARCH_H
#define OP_FLUSH_RESEARCH_H
#include "baseline.h"

/* Prefix views recovered from the flush reads; not complete resource types. */
typedef struct op_flush_face {
    unsigned int unknown_00, flags;
    unsigned char opaque_08[16];
    int *vertex_indices;
} op_flush_face;
typedef struct op_pixel_uv { float u, v; } op_pixel_uv;
typedef struct op_flush_packet {
    struct op_flush_packet *next;
    op_flush_face *face;
    op_projected_vertex *projected;
    op_pixel_uv *uv;
    unsigned int *colors;
    unsigned char corner_count, draw_mode, opacity, render_tag;
    int projected_indices[20], uv_indices[20];
} op_flush_packet;
typedef struct op_texture_dimensions {
    unsigned char opaque_00[12];
    unsigned int width, height;
} op_texture_dimensions;
typedef struct op_flush_texture {
    unsigned char opaque_00[28];
    op_texture_dimensions *dimensions;
    unsigned char opaque_20[12];
    unsigned int handle;
} op_flush_texture;
typedef struct op_flush_material {
    unsigned char opaque_00[176];
    op_flush_texture *first_texture;
} op_flush_material;
typedef struct op_backend_vertex {
    float x, y, z, rhw;
    unsigned int diffuse, specular;
    float u, v;
} op_backend_vertex;

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned int op_projected_fog[];
int op_prepare_material_texture(op_flush_material *material, op_flush_texture *texture, int mip);
unsigned int op_backend_set_flags(unsigned int flags);
void op_backend_bind_texture(void *handle);
/* Mode is a full-width argument; packet storage is only one byte. */
void op_backend_draw_vertices(op_backend_vertex *vertices, int count, int mode);
void *op_backend_queue_vertices(void *handle, unsigned int flags,
    op_backend_vertex *vertices, unsigned int count, int mode);
void op_flush_material_buckets(void);
#ifdef __cplusplus
}
#endif
#endif
