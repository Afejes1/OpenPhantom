#include "api.h"
#include <string.h>
#pragma intrinsic(strcpy)

// FUNCTION: WMAIN 0x0041705b
op_sound_ref_node *op_sound_add_extra_ref(const char *name)
{
    op_sound_ref_node *node;
    op_sound_ref_node *cursor;

    if (op_sound_initialized == 0)
        return 0;
    node = (op_sound_ref_node *)op_sound_allocate_node(64);
    node->flags = 0;
    node->resource = 0;
    node->next = 0;
    strcpy(node->name, name);
    op_sound_insert_name(op_sound_registry, node->name, node);
    ++op_sound_ref_count;
    ++op_sound_extra_count;
    if (op_sound_extra_head == 0) {
        op_sound_extra_head = node;
        return node;
    }
    for (cursor = op_sound_extra_head; cursor->next != 0; cursor = cursor->next) {
    }
    cursor->next = node;
    return node;
}
