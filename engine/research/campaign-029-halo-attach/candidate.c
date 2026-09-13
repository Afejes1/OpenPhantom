#include "api.h"
// FUNCTION: WMAIN 0x0043993e
void op_halo_attach(OP_ACTOR *actor)
{
    unsigned int color = 0xffffff;
    char *model_name = actor->definition->name;
    int index;
    for (index = 0; op_halo_colors[index].name; index++)
    {
        if (!op_compare_names(model_name, op_halo_colors[index].name))
        {
            color = op_halo_colors[index].color;
            break;
        }
    }
    op_halo_add(actor, 9, 0, 2, op_halo_name_a, color, 0);
    op_halo_add(actor, 10, 0, 2, op_halo_name_a, color, 0);
    op_halo_add(actor, 11, 0, 2, op_halo_name_a, color, 0);
    op_halo_add(actor, 12, 0, 2, op_halo_name_a, color, 0);
    op_halo_add(actor, 13, 0, 2, op_halo_name_b, color, 1);
    op_halo_add(actor, 14, 0, 2, op_halo_name_c, color, 1);
}
