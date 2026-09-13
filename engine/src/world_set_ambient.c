#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041f10c
void op_world_set_ambient(OP_B3D_WORLD *world, float ambient)
{
    if (ambient < 0.0)
        ambient = 0.0f;
    else if (ambient > 1.0)
        ambient = 1.0f;
    world->ambient = ambient;
}