#include "api.h"
// FUNCTION: WMAIN 0x0043d8ca
void op_zap_draw_opaque(OP_VEC3 *start, OP_VEC3 *end, OP_VEC3 *snapshot, int depth, float width)
{
    int i;
    void *material;
    int num_points;
    op_generate_zap_points(start, end, depth);
    num_points = (1 << depth) + 1;
    for (i = 0; i < num_points; i++)
        snapshot[i] = op_zap_points[i];
    material = op_resolve_zap_material(op_zap_sprite);
    op_submit_zap_points(snapshot, num_points, 0xfff0dcff, material, width);
}
