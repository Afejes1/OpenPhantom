#include "api.h"
// FUNCTION: WMAIN 0x0043d784
void op_zap_subdivide(int start, int end, int depth, float amplitude)
{
    int middle;
    if (depth == 0)
        return;
    middle = (start + end) / 2;
    op_zap_points[middle].x = (op_zap_points[start].x + op_zap_points[end].x +
                               (op_zap_half - (float)op_random() * op_random_scale) * amplitude) *
                              op_zap_half;
    op_zap_points[middle].y = (op_zap_points[start].y + op_zap_points[end].y +
                               (op_zap_half - (float)op_random() * op_random_scale) * amplitude) *
                              op_zap_half;
    op_zap_points[middle].z = (op_zap_points[start].z + op_zap_points[end].z +
                               (op_zap_half - (float)op_random() * op_random_scale) * amplitude) *
                              op_zap_half;
    depth--;
    amplitude *= op_zap_decay;
    op_zap_subdivide(start, middle, depth, amplitude);
    op_zap_subdivide(middle, end, depth, amplitude);
}
