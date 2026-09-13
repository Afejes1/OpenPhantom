#include "api.h"
// FUNCTION: WMAIN 0x0041e373
void op_world_free_scripts(OP_WORLD *world)
{
    OP_SCRIPT *script;
    int index;
    for (index = 0; index < world->script_count; ++index) {
        script = world->scripts[index];
        op_free_script_data(script);
    }
}
