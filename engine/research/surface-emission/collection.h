#ifndef OP_STATIC_CELL_RESEARCH_H
#define OP_STATIC_CELL_RESEARCH_H
#include "batch_surface.h"
#include "culling.h"
/* The variable-length surface records immediately follow this 16-byte header. */
typedef struct op_static_cell {
    unsigned char surface_count, opaque_01[15];
} op_static_cell;
#ifdef __cplusplus
extern "C" {
#endif
extern float op_cell_camera_transform[12];
extern float op_cell_vertical_slope, op_cell_horizontal_slope;
extern float op_cell_vertical_padding, op_cell_horizontal_padding, op_cell_far_limit;
extern unsigned int op_room_skipped_extra_count, op_frustum_skipped_extra_count, op_frustum_group_count;
void op_transform_cell_point(op_scan_vector *output, const op_scan_vector *input, const float *transform);
void op_gather_static_cell(op_static_cell *cell, float center_x, float center_y);
#ifdef __cplusplus
}
#endif
#endif
