#include "utilities.h"
// FUNCTION: WMAIN 0x004977f1
int op_rect_overlap_inclusive(const OP_INT_RECT *left, const OP_INT_RECT *right)
{
    if ((int)((unsigned int)left->x + (unsigned int)left->width) < right->x)
        return 0;
    if ((int)((unsigned int)right->x + (unsigned int)right->width) < left->x)
        return 0;
    if ((int)((unsigned int)left->y + (unsigned int)left->height) < right->y)
        return 0;
    if ((int)((unsigned int)right->y + (unsigned int)right->height) < left->y)
        return 0;
    return 1;
}
