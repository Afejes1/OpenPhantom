#include "sprite_resources.h"
// FUNCTION: WMAIN 0x004295f4
int op_release_sprite(void **slot)
{
    void *resource;
    if (!slot)
        return 0;
    if (!*slot)
        return 0;
    resource = *slot;
    op_release_resource(resource);
    resource = 0;
    *slot = 0;
    return 1;
}
