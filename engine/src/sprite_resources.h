#ifndef OP_SPRITE_RESOURCES_H
#define OP_SPRITE_RESOURCES_H
#include <stddef.h>
/* Partial views; original allocation extents remain unknown. */
typedef struct OP_MATERIAL
{
    unsigned char prefix[176];
    unsigned char *frame;
} OP_MATERIAL;
typedef struct OP_SPRITE
{
    unsigned char prefix[132];
    OP_MATERIAL *material;
} OP_SPRITE;
typedef char sprite_offsets
    [(sizeof(void *) == 4 && offsetof(OP_SPRITE, material) == 132 && offsetof(OP_MATERIAL, frame) == 176) ? 1 : -1];
int op_prepare_material_frame(OP_MATERIAL *, unsigned char *, int);
void *op_resolve_zap_material(void *sprite);
extern char op_sprite_format[];
int op_format_sprite_name(char *, char *, ...);
void *op_acquire_resource(unsigned int, char *);
void *op_acquire_sprite(char *);
void op_release_resource(void *);
int op_release_sprite(void **);
#endif
