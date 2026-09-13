#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041eabc
#include <string.h>
#pragma intrinsic(memcpy)
void op_world_mangle_names(OP_B3D_WORLD *world)
{
    char name_buffer[32];
    int i, j;
    OP_B3D_ACTOR *actor;
    if (world == 0)
        return;
    for (i = 0; i < (int)world->animation_name_count; ++i)
    {
        actor = world->actor_resources[i];
        memcpy(name_buffer, actor->name, sizeof(name_buffer));
        name_buffer[31] = 0;
        for (j = 0; j < 32; ++j)
        {
            if (name_buffer[j] == '.')
                name_buffer[j] = '_';
            if (name_buffer[j] == 0)
                break;
        }
    }
}
