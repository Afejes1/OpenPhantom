#include "api.h"

// FUNCTION: WMAIN 0x00406eed
void op_light_disable_world(OP_LIGHT_WORLD *world, int index)
{
    OP_LIGHT_RECORD *record;

    if (world == 0 || index < 0 || index >= world->count)
        return;
    record = &world->records[index];
    if (record->active != 0)
        op_light_deactivate_record(world, 1, record);
}
