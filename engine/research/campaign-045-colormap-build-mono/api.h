#ifndef OP_CAMPAIGN045_API_H
#define OP_CAMPAIGN045_API_H
extern const float op_colour_one;
unsigned int op_palette_nearest(unsigned char *, unsigned int, float, float, float);
int op_colormap_build_mono(unsigned char *, unsigned char *, unsigned char, unsigned char, unsigned char);
int op_colormap_build_blend(unsigned char *, unsigned char *, float);
#endif
