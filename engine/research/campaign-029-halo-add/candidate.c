#include "api.h"
// FUNCTION: WMAIN 0x00439857
void op_halo_add(OP_ACTOR *actor, unsigned int key, int field8, int fieldc, char *name, unsigned int color, int mode)
{
    int node_index;
    OP_HALO *current;
    int i;
    if (!actor || !name)
        return;
    node_index = op_find_node_ordinal(actor, key);
    if (node_index <= 0)
        return;
    for (i = 0; i < 32; i++)
    {
        if (!op_halos[i].owner)
            break;
    }
    if (i >= 32)
        return;
    current = &op_halos[i];
    current->sprite = op_acquire_sprite(name);
    if (!current->sprite)
        return;
    current->node = node_index;
    current->owner = actor;
    current->field8 = field8;
    current->fieldc = fieldc;
    current->color = color;
    current->mode = mode;
    op_halo_count++;
    actor->flags |= 0x10;
}
