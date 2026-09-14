#ifndef OP_COLORMAP_STATE_H
#define OP_COLORMAP_STATE_H
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
typedef char colormap_extent[(sizeof(OP_COLORMAP) == 844 && offsetof(OP_COLORMAP, palette) == 48 &&
                              offsetof(OP_COLORMAP, flags) == 32 && offsetof(OP_COLORMAP, handle334) == 820 &&
                              offsetof(OP_COLORMAP, handle33c) == 828)
                                 ? 1
                                 : -1];
extern OP_COLORMAP *op_colormap_current, *op_colormap_hardware;
extern int op_palette_device_count;
void op_install_palette(void *);
int op_colormap_set_current(OP_COLORMAP *);
int op_colormap_set_hardware(OP_COLORMAP *);
void op_colormap_free_entry(OP_COLORMAP *);
OP_COLORMAP *op_rd_colormap_load(const char *);
int op_colormap_load_entry(const char *, OP_COLORMAP *);
void *op_allocate(unsigned int);
void op_rd_colormap_free(void *);
void op_release(void *);
#endif
