#include "api.h"
// FUNCTION: WMAIN 0x00414eb7
int op_palette_resource_release(void)
{
    op_release_resource(op_palette_resource_current);
    op_palette_resource_current = 0;
    return 1;
}
