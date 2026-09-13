#include "api.h"

// FUNCTION: WMAIN 0x00416589
void op_sound_free_extra_refs(void)
{
    op_sound_ref_node *old;
    op_sound_ref_node *node;

    old = op_sound_extra_head;
    while (old != 0) {
        op_sound_mark_resource(old->resource, 1);
        op_sound_release_resource(old->resource);
        old->resource = 0;
        op_sound_remove_name(op_sound_registry, old->name);
        --op_sound_ref_count;
        node = old;
        old = old->next;
        op_sound_free_node(node);
        node = 0;
        --op_sound_extra_count;
    }
    op_sound_extra_head = 0;
}