#include "api.h"
// FUNCTION: WMAIN 0x0041f02b
void op_world_free_tail_blocks(OP_WORLD *world)
{
    if (world == 0) return;
    if (world->sprite_count != 0) {
        op_release(world->sprites);
        world->sprites = 0;
    }
    world->sprite_count = 0;
    if (world->sound_placements != 0) {
        op_release(world->sound_placements);
        world->sound_placements = 0;
    }
    if (world->sound_descriptors != 0) {
        op_release(world->sound_descriptors);
        world->sound_descriptors = 0;
    }
}
