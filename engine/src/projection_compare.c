#include "projected_overlay.h"
#include <string.h>
// FUNCTION: WMAIN 0x0041C9A0
int op_projection_compare(const void *left, const void *right)
{
    const OP_PROJECTION *a = (const OP_PROJECTION *)left;
    const OP_PROJECTION *b = (const OP_PROJECTION *)right;
    if (a->surface < b->surface)
        return -1;
    if (a->surface > b->surface)
        return 1;
    if (a->sequence < b->sequence)
        return -1;
    return a->sequence > b->sequence;
}
