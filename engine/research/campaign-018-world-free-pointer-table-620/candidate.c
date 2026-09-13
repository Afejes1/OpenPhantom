#include "api.h"
// FUNCTION: WMAIN 0x0041efb5
void op_world_free_pointer_table_620(OP_WORLD *world)
{
    int index;
    if (world == 0) return;
    for (index = 0; index < world->count; ++index) {
        if (world->entries[index] != 0) {
            op_release(world->entries[index]);
            world->entries[index] = 0;
        }
    }
    world->count = 0;
}
