#include "projected_overlay.h"
#include <string.h>
// FUNCTION: WMAIN 0x0041C8B0
void op_projection_expire(void)
{
    OP_PROJECTION *p = op_projections;
    int count;
    for (count = 255; count > 0; count--, p++)
    {
        if (p->surface && p->lifetime != op_projection_zero && op_elapsed - p->created > p->lifetime)
        {
            memset(p, 0, sizeof(*p));
            op_projection_dirty = 1;
        }
    }
}
