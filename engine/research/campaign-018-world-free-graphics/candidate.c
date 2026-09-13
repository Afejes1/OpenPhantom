#include "api.h"
// FUNCTION: WMAIN 0x0041cd40
int op_world_free_graphics(OP_WORLD *world)
{
    if (world == 0) return 1;
    if (world->load_state != 0) {
        op_world_free_materials(world);
        op_world_free_pointer_table_220(world);
        op_world_free_pointer_table_620(world);
    }
    return 0;
}
