#include "light.h"
// FUNCTION: WMAIN 0x0047a790
OP_LIGHT *op_light_new(void)
{
    OP_LIGHT *light;
    light = (OP_LIGHT *)op_allocate(sizeof(OP_LIGHT));
    if (!light)
        return 0;
    op_light_init(light);
    return light;
}
