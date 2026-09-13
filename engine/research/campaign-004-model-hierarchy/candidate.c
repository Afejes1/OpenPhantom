#include "api.h"

// FUNCTION: WMAIN 0x004100a0
void op_queue_model_hierarchy(op_hierarchy_node *node)
{
    op_hierarchy_node *child;
    unsigned int index;

    if (node->mesh_index != -1 &&
        op_hierarchy_thing->hidden_meshes[node->mesh_index] == 0) {
        op_queue_mesh_faces(op_hierarchy_geoset->meshes + node->mesh_index,
                            op_hierarchy_thing->matrices[node->ordinal]);
    }
    if (node->child_count != 0) {
        child = node->first_child;
        for (index = 0; index < node->child_count; ++index) {
            if (op_hierarchy_thing->amputated_nodes[child->ordinal] == 0)
                op_queue_model_hierarchy(child);
            child = child->next_sibling;
        }
    }
}