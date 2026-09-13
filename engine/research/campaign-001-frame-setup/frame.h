#ifndef OP_FRAME_SETUP_RESEARCH_H
#define OP_FRAME_SETUP_RESEARCH_H

#include "baseline.h"

/* Partial layouts: only fields observed in the 0x00404180 body are named. */
typedef struct op_frame_viewport {
    unsigned char opaque_00[0x18];
    int left;
    int top;
    int right;
    int bottom;
} op_frame_viewport;

typedef struct op_frame_camera_transform {
    unsigned char opaque_00[4];
    op_frame_viewport *viewport;
    float basis[12];
    float half_span;
} op_frame_camera_transform;

typedef struct op_frame_camera {
    unsigned char opaque_00[0x2c];
    float eye_z;
    unsigned char opaque_30[4];
    float pitch_degrees;
} op_frame_camera;

typedef struct op_frame_world {
    unsigned char opaque_00[0x78];
    int lower_z;
    int upper_z;
} op_frame_world;

typedef struct op_scan_line {
    int count, index, x, y;
    int reserved_10, reserved_14;
    int start_x, start_y, end_x, end_y;
    int major_delta, minor_delta, error;
    int diagonal_x, diagonal_y, diagonal_error;
    int straight_x, straight_y, straight_error;
} op_scan_line;

#ifdef __cplusplus
extern "C" {
#endif

extern op_frame_camera_transform *op_frame_transform;
extern op_frame_camera *op_frame_camera_state;
extern float op_scan_transform[12];
extern float op_scan_bounds[4];
extern int op_scan_octant;
extern int op_scan_octant_table[][8];
extern int op_scan_axis_flags[16];
extern float op_scan_direction_x, op_scan_direction_y;
extern int op_scan_direction_step_x, op_scan_direction_step_y;
extern float op_scan_position[3];
extern int op_scan_x, op_scan_y, op_scan_anchor_x, op_scan_anchor_y;
extern int op_scan_limit, op_scan_bias, op_scan_mode, op_scan_mode_enabled;
extern int op_scan_quadrant;
extern int op_scan_offsets[511][2];
extern int op_scan_offset_index;
extern int op_scan_turn_x, op_scan_turn_y, op_scan_turn_z;
extern int op_scan_pending, op_scan_cursor, op_scan_low_mark, op_scan_high_mark;
extern op_scan_line op_frame_scan_line;

void op_sin_cos_degrees(float angle, float *cosine_out, float *sine_out);
void op_init_scan_line(op_scan_line *line, int x0, int y0, int x1, int y1);
void op_step_scan(float *position, int x_step, int y_step);
void op_frame_setup(op_frame_world *world);

#ifdef __cplusplus
}
#endif
#endif
