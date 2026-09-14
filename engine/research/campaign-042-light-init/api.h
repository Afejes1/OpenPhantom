#ifndef OP_CAMPAIGN042_API_H
#define OP_CAMPAIGN042_API_H
#include <stddef.h>
typedef struct OP_COLORMAP
{
    unsigned char prefix[32];
    unsigned int flags;
    unsigned char unknown24[12];
    unsigned char palette[1];
    unsigned char unknown31[771];
    void *handle334;
    unsigned int unknown338;
    void *handle33c;
    unsigned char unknown340[12];
} OP_COLORMAP;
typedef struct OP_LIGHT
{
    unsigned int kind, enabled;
    float x, y, z, intensity;
    unsigned int colour;
    float near_range, far_range;
    unsigned int unknown24, unknown28;
    float falloff, red, green, blue;
    unsigned int unknown3c;
} OP_LIGHT;
typedef char colormap_layout[(sizeof(OP_COLORMAP) == 844 && offsetof(OP_COLORMAP, flags) == 32 &&
                              offsetof(OP_COLORMAP, palette) == 48 && offsetof(OP_COLORMAP, handle334) == 820 &&
                              offsetof(OP_COLORMAP, handle33c) == 828)
                                 ? 1
                                 : -1];
typedef char
    light_layout[(sizeof(OP_LIGHT) == 64 && offsetof(OP_LIGHT, falloff) == 44 && offsetof(OP_LIGHT, blue) == 56) ? 1
                                                                                                                 : -1];
void *op_allocate(unsigned int);
void op_release(void *);
void *op_rd_colormap_load(const char *);
void op_rd_colormap_free(void *);
int op_colormap_load_entry(const char *, OP_COLORMAP *);
void op_colormap_free_entry(OP_COLORMAP *);
OP_LIGHT *op_light_new(void);
int op_light_init(OP_LIGHT *);
void op_light_free(OP_LIGHT *);
void op_light_free_inner(OP_LIGHT *);
#endif
