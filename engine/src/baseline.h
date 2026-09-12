#ifndef OPENPHANTOM_BASELINE_H
#define OPENPHANTOM_BASELINE_H

/* Partial layouts only. Names describe observed accesses, not original symbols. */
typedef struct op_viewport {
    unsigned char opaque_00[8];
    unsigned int width_bits;
    unsigned int height_bits;
    unsigned char opaque_10[8];
    int edge_18;
    int edge_1c;
    int edge_20;
    int edge_24;
} op_viewport;

typedef struct op_frustum {
    unsigned char opaque_00[4];
    float near_bound;
    float far_bound;
} op_frustum;

typedef struct op_camera_state {
    unsigned char opaque_00[4];
    op_viewport *viewport;
    unsigned char opaque_08[52];
    float focal;
    unsigned char opaque_40[8];
    op_frustum *frustum;
} op_camera_state;

typedef struct op_face_indices {
    unsigned char opaque_00[20];
    unsigned int vertex_count;
    const unsigned int *vertex_indices;
} op_face_indices;

typedef struct op_render_thing {
    unsigned char opaque_00[0x150];
    float render_opacity;
} op_render_thing;

extern op_render_thing *op_active_render_thing;
extern float op_mesh_opacity;
extern unsigned char op_grid[];
extern op_camera_state *op_camera;
extern unsigned int op_width_bits, op_height_bits;
extern float op_focal, op_depth_scale;
extern float op_edge_18, op_edge_1c, op_edge_20, op_edge_24;
extern float op_far, op_near;
extern int op_depth_mode;

int op_material_mode(int mode);
unsigned char *op_grid_cell(int x, int y);
float op_plane_coordinate(int axis, float *vertex, float *normal, float *point);
void op_update_projection(void);
unsigned int op_face_clip_flags(const op_face_indices *face,
                               const unsigned char *clip_codes);
void op_transform_project(const float *vertices, float *output,
                          const int *indices, unsigned char *codes,
                          int count, const float *matrix);

void op_set_mesh_render_thing(op_render_thing *thing);

/* Queue layouts established by the allocation, submission and flush routines. */
typedef struct op_projected_vertex {
    float x, y, reciprocal_depth, mapped_depth;
} op_projected_vertex;

typedef struct op_packet_face {
    unsigned char opaque_00[0x20];
    void *material;
} op_packet_face;

typedef struct op_face_packet {
    struct op_face_packet *next;
    op_packet_face *face;
    unsigned char opaque_08[0x0f];
    unsigned char render_tag;
    unsigned char opaque_18[0xa0];
} op_face_packet;

typedef struct op_material_bucket {
    void *material;
    op_face_packet *head, *tail;
} op_material_bucket;

extern unsigned int op_projected_vertex_count;
extern op_projected_vertex op_projected_vertices[];
extern int op_face_packet_count, op_material_bucket_count;
extern op_face_packet op_face_packets[];
extern op_material_bucket op_material_buckets[];
extern op_material_bucket *op_last_material_bucket;
extern unsigned char op_render_queue_tag;

op_projected_vertex *op_peek_projected_vertices(unsigned int requested);
op_face_packet *op_peek_face_packet(void);
int op_queue_face_packet(op_face_packet *packet);

/* The callee currently consumes world; retain all three original ABI arguments. */
extern unsigned int op_gathered_surface_count;
extern void *op_gathered_world;
extern int op_last_submitted_surface_count;
int op_draw_surface_buckets(void *world, const void *camera_data, unsigned int count);
void op_submit_gathered_surfaces(void);

#endif
