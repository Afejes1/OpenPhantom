#include "api.h"
// FUNCTION: WMAIN 0x0041cc80
int op_world_destroy(OP_WORLD *world)
{
    if (world == 0) return 1;
    if (world->loaded != 0) {
        if (world->actor_count != 0 && world->actor != 0) {
            op_release(world->actor);
            world->actor = 0;
        }
        op_world_free_materials(world);
        op_world_free_pointer_table_220(world);
        op_world_free_pointer_table_620(world);
        op_world_free_tail_blocks(world);
        op_world_free_scripts(world);
        if (world->retained != 0)
            op_release(world->retained);
        op_release(world);
    }
    return 0;
}