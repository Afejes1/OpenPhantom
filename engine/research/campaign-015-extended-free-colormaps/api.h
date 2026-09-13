#ifndef OP_C015_EXTENDED_FREE_COLORMAPS_API_H
#define OP_C015_EXTENDED_FREE_COLORMAPS_API_H
extern int op_extended_colormap_count;extern void *op_extended_colormaps[128];
void op_rd_colormap_free(void *colormap);void op_extended_free_colormaps(void);
#endif
