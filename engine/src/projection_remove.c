#include "projected_overlay.h"
#include <string.h>
// FUNCTION: WMAIN 0x0041c910
void op_projection_remove(int token)
{
    OP_PROJECTION *p = op_projections;
    int count;
    if (token == 0)
        return;
    for (count = 255; count > 0; count--, p++)
    {
        if (p->group_token == token)
        {
            op_projection_dirty = 1;
            memset(p, 0, sizeof(*p));
        }
    }
}
