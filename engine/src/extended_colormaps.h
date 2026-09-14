#ifndef OP_EXTENDED_COLORMAPS_H
#define OP_EXTENDED_COLORMAPS_H
#include "colormap_state.h"

extern int op_extended_colormap_count;
extern void *op_extended_colormaps[128];

OP_COLORMAP *op_rd_colormap_load(const char *name);
void op_rd_colormap_free(void *colormap);
void *op_extended_load_colormap(const char *name);
void op_extended_free_colormaps(void);

#endif
