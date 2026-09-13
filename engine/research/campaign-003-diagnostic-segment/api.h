#ifndef OP_DIAGNOSTIC_SEGMENT_API_H
#define OP_DIAGNOSTIC_SEGMENT_API_H
#include "baseline.h"
typedef struct op_point3 { float x, y, z; } op_point3;
void op_submit_polyline(const float *, int, unsigned int, void *, float);
void op_draw_diagnostic_segment(const op_point3 *, const op_point3 *);
#endif