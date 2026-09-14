#include "utilities.h"
// FUNCTION: WMAIN 0x004978bd
void op_rect_union(OP_INT_RECT *left, const OP_INT_RECT *right)
{
    if (left->x > right->x)
    {
        left->width = (int)((unsigned int)left->width + ((unsigned int)left->x - (unsigned int)right->x));
        left->x = right->x;
    }
    if (left->y > right->y)
    {
        left->height = (int)((unsigned int)left->height + ((unsigned int)left->y - (unsigned int)right->y));
        left->y = right->y;
    }
    if ((int)((unsigned int)left->x + (unsigned int)left->width) <
        (int)((unsigned int)right->x + (unsigned int)right->width))
        left->width = (int)(((unsigned int)right->x + (unsigned int)right->width) - (unsigned int)left->x);
    if ((int)((unsigned int)left->y + (unsigned int)left->height) <
        (int)((unsigned int)right->y + (unsigned int)right->height))
        left->height = (int)(((unsigned int)right->y + (unsigned int)right->height) - (unsigned int)left->y);
}
