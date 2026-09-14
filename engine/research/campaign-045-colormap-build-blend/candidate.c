#include "api.h"
// FUNCTION: WMAIN 0x00478d11
int op_colormap_build_blend(unsigned char *table, unsigned char *palette, float blend)
{
    float inner_g, outer_b, outer_r;
    unsigned char *inner;
    int outer_index;
    unsigned char *outer;
    float result_b, result_r, outer_g, inner_b, inner_r;
    int inner_index;
    float result_g;
    outer = palette;
    for (outer_index = 0; outer_index < 256; ++outer_index)
    {
        outer_r = (float)*outer++;
        outer_g = (float)*outer++;
        outer_b = (float)*outer++;
        inner = palette;
        for (inner_index = 0; inner_index < 256; ++inner_index)
        {
            inner_r = (float)*inner++;
            inner_g = (float)*inner++;
            inner_b = (float)*inner++;
            result_r = outer_r * blend + (op_colour_one - blend) * inner_r;
            result_g = outer_g * blend + (op_colour_one - blend) * inner_g;
            result_b = outer_b * blend + (op_colour_one - blend) * inner_b;
            (table + outer_index * 256)[inner_index] =
                (unsigned char)((unsigned char)op_palette_nearest(palette + 3, 254, result_r, result_g, result_b) + 1);
        }
    }
    return 1;
}
