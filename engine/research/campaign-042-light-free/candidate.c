#include "api.h"
// FUNCTION: WMAIN 0x0047a849
void op_light_free(OP_LIGHT *light)
{
    if (light)
    {
        op_light_free_inner(light);
        op_release(light);
    }
}
