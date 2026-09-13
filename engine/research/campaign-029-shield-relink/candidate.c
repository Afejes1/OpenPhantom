#include "api.h"
// FUNCTION: WMAIN 0x0043cc28
void op_shield_relink(void)
{
    OP_SHIELD *current;
    void *object_iterator;
    int i;
    int index;
    OP_ACTOR *actor;
    object_iterator = op_iterator_open(op_object_pool);
    for (i = 0; i < 32; i++)
    {
        current = &op_shields[i];
        if (!current->active)
            continue;
        index = current->saved_actor;
        op_iterator_reset(object_iterator);
        while ((actor = op_iterator_next(object_iterator)) != 0)
        {
            if (actor->slot == index)
            {
                actor->slot = i;
                current->actor = actor;
                current->saved_actor = -1;
                break;
            }
        }
    }
    op_iterator_close(object_iterator);
}
