#ifndef OP_LIGHT_H
#define OP_LIGHT_H
#include <stddef.h>
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

typedef char op_light_layout
    [(sizeof(OP_LIGHT) == 64 && offsetof(OP_LIGHT, falloff) == 44 && offsetof(OP_LIGHT, blue) == 56) ? 1 : -1];
void *op_allocate(unsigned int);
void op_release(void *);
OP_LIGHT *op_light_new(void);
int op_light_init(OP_LIGHT *);
void op_light_free(OP_LIGHT *);
void op_light_free_inner(OP_LIGHT *);
#endif
