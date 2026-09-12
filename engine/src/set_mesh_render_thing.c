#include "baseline.h"

/* FUNCTION: WMAIN 0x0040f1b0 */
void op_set_mesh_render_thing(op_render_thing *thing)
{
    op_active_render_thing = thing;
    if (thing != 0) {
        op_mesh_opacity = thing->render_opacity * 255.0f;
        return;
    }
    op_mesh_opacity = 255.0f;
}
