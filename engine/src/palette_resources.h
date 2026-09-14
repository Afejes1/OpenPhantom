#ifndef OP_PALETTE_RESOURCES_H
#define OP_PALETTE_RESOURCES_H
#include <stddef.h>
#include "colormap_io.h"
#include "material_state.h"

extern void *op_palette_pool;
extern OP_COLORMAP *op_palette_resource_current;
extern char op_palette_resource_name[];
void *op_pool_allocate(void *, unsigned int);
void op_pool_release(void *);
int op_colormap_load_entry(const char *, OP_COLORMAP *);
void op_colormap_free_entry(OP_COLORMAP *);
int op_colormap_set_current(OP_COLORMAP *);
void op_material_install_palette(OP_MATERIAL *);
void *op_acquire_resource(unsigned int, char *);
void op_release_resource(void *);
OP_COLORMAP *op_palette_resource_load(const char *);
void op_palette_resource_free(void *);
int op_palette_resource_install(void);
OP_COLORMAP *op_palette_resource_get(void);
int op_palette_resource_set_name(const char *);
#endif
