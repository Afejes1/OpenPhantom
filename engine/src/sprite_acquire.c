#include "sprite_resources.h"
// FUNCTION: WMAIN 0x00429540
void *op_acquire_sprite(char *base_name)
{
    char name[80];
    void *result;
    op_format_sprite_name(name, op_sprite_format, base_name);
    result = op_acquire_resource(0x53505254, name);
    if (!result)
        return 0;
    return result;
}
