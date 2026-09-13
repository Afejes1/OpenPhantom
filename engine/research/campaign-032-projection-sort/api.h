#ifndef OP_CAMPAIGN032_H
#define OP_CAMPAIGN032_H
#include <stddef.h>
typedef struct OP_PROJECTION
{
    float created;
    unsigned char unknown04[4];
    unsigned int surface;
    unsigned char unknown0c[40];
    float lifetime;
    unsigned char unknown38[4];
    int group_token;
    unsigned int sequence;
} OP_PROJECTION;
typedef char
    op_projection_layout[(sizeof(void *) == 4 && sizeof(OP_PROJECTION) == 68 && offsetof(OP_PROJECTION, surface) == 8 &&
                          offsetof(OP_PROJECTION, lifetime) == 52 && offsetof(OP_PROJECTION, group_token) == 60 &&
                          offsetof(OP_PROJECTION, sequence) == 64)
                             ? 1
                             : -1];
extern OP_PROJECTION op_projections[255];
extern int op_projection_dirty;
extern float op_elapsed;
void op_projection_expire(void);
void op_projection_remove(int);
void op_projection_clear(void);
void op_projection_sort(void);
int op_projection_compare(const void *, const void *);
void op_projection_qsort(void *, unsigned int, unsigned int, int (*)(const void *, const void *));
#endif
