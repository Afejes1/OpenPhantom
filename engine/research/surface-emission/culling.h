#ifndef OP_SCAN_CULL_RESEARCH_H
#define OP_SCAN_CULL_RESEARCH_H
#include "baseline.h"
typedef struct op_scan_vector { float x, y, z; } op_scan_vector;
typedef struct op_scan_camera {
    op_camera_state prefix;
    void (*project)(op_scan_vector *output, const op_scan_vector *input);
} op_scan_camera;
#ifdef __cplusplus
extern "C" {
#endif
extern float op_scan_step_z[3];
extern int op_scan_corner_pairs[4][4];
extern float op_scan_edge_18, op_scan_edge_20, op_scan_edge_1c;
int op_cull_scan_plane(int plane, const float *position, float height);
#ifdef __cplusplus
}
#endif
#endif
