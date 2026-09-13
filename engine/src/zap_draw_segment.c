#include "zap_effects.h"
// FUNCTION: WMAIN 0x0043d648
void op_zap_draw_segment(OP_VEC3 *start, OP_VEC3 *end, int depth, float width, unsigned int color)
{
    void *material;
    int num_points;
    material = op_resolve_zap_material(op_zap_sprite);
    if (depth >= 4)
        depth = 4;
    op_generate_zap_points(start, end, depth);
    num_points = (1 << depth) + 1;
    op_submit_zap_points(op_zap_points, num_points, color, material, width);
}
