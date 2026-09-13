#include "projected_overlay.h"
#include <string.h>
// FUNCTION: WMAIN 0x0041C950
void op_projection_clear(void)
{
    memset(op_projections, 0, sizeof(op_projections));
}
