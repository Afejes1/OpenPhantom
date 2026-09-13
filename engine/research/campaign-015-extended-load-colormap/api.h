#ifndef OP_C015_EXTENDED_LOAD_COLORMAP_API_H
#define OP_C015_EXTENDED_LOAD_COLORMAP_API_H
extern int op_extended_colormap_count;
extern void *op_extended_colormaps[128];
void *op_rd_colormap_load(const char *name);
void *op_extended_load_colormap(const char *name);
#endif
