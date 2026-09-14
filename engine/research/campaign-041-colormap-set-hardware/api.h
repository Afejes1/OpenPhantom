#ifndef OP_CAMPAIGN041_API_H
#define OP_CAMPAIGN041_API_H
#include <stddef.h>
typedef struct OP_COLORMAP
{
    unsigned char prefix[48];
    unsigned char palette[1];
    unsigned char unknown31[795];
} OP_COLORMAP;
typedef char colormap_extent[(sizeof(OP_COLORMAP) == 844 && offsetof(OP_COLORMAP, palette) == 48) ? 1 : -1];
extern OP_COLORMAP *op_colormap_current, *op_colormap_hardware;
extern int op_palette_device_count;
void op_install_palette(void *);
int op_colormap_set_current(OP_COLORMAP *);
int op_colormap_set_hardware(OP_COLORMAP *);
void op_colormap_free_entry(void *);
void op_rd_colormap_free(void *);
void op_release(void *);
#endif
