#include "api.h"
// FUNCTION: WMAIN 0x00478b65
int op_colormap_build_mono(unsigned char *table, unsigned char *palette, unsigned char red, unsigned char green,
                           unsigned char blue)
{
    float levels, delta_r, value_b, value_r, delta_g;
    int colour;
    unsigned char *cursor;
    float target_b, target_r, value_g, delta_b;
    int level;
    float target_g;
    target_r = (float)red;
    target_g = (float)green;
    target_b = (float)blue;
    levels = 63.0f;
    for (colour = 0; colour < 32; ++colour)
        for (level = 0; level < 64; ++level)
            (table + level * 256)[colour] = (unsigned char)colour;
    cursor = palette + 96;
    for (colour = 32; colour < 256; ++colour)
    {
        value_r = (float)*cursor++;
        value_g = (float)*cursor++;
        value_b = (float)*cursor++;
        delta_r = (target_r - value_r) / levels;
        delta_g = (target_g - value_g) / levels;
        delta_b = (target_b - value_b) / levels;
        table[0x3f00 + colour] = (unsigned char)colour;
        for (level = 62; level >= 0; --level)
        {
            value_r += delta_r;
            value_g += delta_g;
            value_b += delta_b;
            (table + level * 256)[colour] =
                (unsigned char)((unsigned char)op_palette_nearest(palette + 3, 254, value_r, value_g, value_b) + 1);
        }
    }
    return 1;
}
