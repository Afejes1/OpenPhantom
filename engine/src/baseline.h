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

#endif
