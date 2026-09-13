#include "api.h"
// FUNCTION: WMAIN 0x0043d5f5
void op_zap_draw_default(float *start, float *end)
{
    void *material;
    int num_points;
    material = op_resolve_zap_material(op_zap_sprite);
    op_generate_zap_points(start, end, 4);
    num_points = 17;
    op_submit_zap_points(op_zap_points, num_points, 0xb4c800ffU, material, 4.0f);
}
