#ifndef OP_CAMPAIGN055_API_H
#define OP_CAMPAIGN055_API_H
#include <stddef.h>
typedef struct OP_INT_RECT
{
    int x, y, width, height;
} OP_INT_RECT;
typedef char rect_layout[(sizeof(OP_INT_RECT) == 16 && offsetof(OP_INT_RECT, y) == 4 &&
                          offsetof(OP_INT_RECT, width) == 8 && offsetof(OP_INT_RECT, height) == 12)
                             ? 1
                             : -1];
int op_rect_overlap_inclusive(const OP_INT_RECT *, const OP_INT_RECT *);
int op_rect_overlap_strict(const OP_INT_RECT *, const OP_INT_RECT *);
void op_rect_union(OP_INT_RECT *, const OP_INT_RECT *);
unsigned int op_rotating_checksum(const unsigned char *, unsigned int, unsigned int);
#endif
