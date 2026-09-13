#include "api.h"
// FUNCTION: WMAIN 0x0041eeaa
void op_world_free_materials(OP_WORLD *world)
{
    int index;
    if (world == 0 || world->load_state == 0) return;
    for (index = 0; index < world->texture_count; ++index) {
        op_destroy_material(world->primary[index]);
        world->primary[index] = 0;
        op_destroy_material(world->secondary[index]);
        world->secondary[index] = 0;
    }
    world->texture_count = 0;
}
