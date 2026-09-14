#ifndef OP_CAMPAIGN049_API_H
#define OP_CAMPAIGN049_API_H
#include <stddef.h>
typedef struct OP_COLORMAP
{
    char name[32];
    unsigned int flags, field24, field28, field2c;
    unsigned char palette[768];
    unsigned char *light_table;
    void *handle334;
    unsigned char *alpha_table;
    void *handle33c;
    unsigned char tail[12];
} OP_COLORMAP;
typedef struct OP_MATERIAL OP_MATERIAL;
typedef char map_layout[(sizeof(OP_COLORMAP) == 844 && offsetof(OP_COLORMAP, palette) == 48) ? 1 : -1];
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
int op_palette_resource_release(void);
#endif
