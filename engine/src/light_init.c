#include "light.h"
// FUNCTION: WMAIN 0x0047a7be
int op_light_init(OP_LIGHT *light)
{
    light->kind = 2;
    light->enabled = 1;
    light->x = 0.0f;
    light->y = 0.0f;
    light->z = 0.0f;
    light->intensity = 1.0f;
    light->colour = 0x00ffffff;
    light->red = 1.0f;
    light->green = 1.0f;
    light->blue = 1.0f;
    light->falloff = 0.2f;
    light->near_range = 0.0f;
    light->far_range = 0.0f;
    return 1;
}
