#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static int checks, failures, calls;
static op_hierarchy_mesh *got_mesh[16];
static float *got_matrix[16];
static op_hierarchy_mesh meshes[8], other_meshes[8];
static float matrices[8][12], other_matrices[8][12];
static int amputated[8], hidden[8], other_amputated[8], other_hidden[8];
static op_hierarchy_thing_state thing, other_thing;
static op_hierarchy_geoset_state geoset, other_geoset;
op_hierarchy_thing_state *op_hierarchy_thing = &thing;
op_hierarchy_geoset_state *op_hierarchy_geoset = &geoset;
static op_hierarchy_node *mutate_parent;
static unsigned int mutate_count;
static int mutate_on_call, switch_on_call;

static void check(int ok, const char *description)
{
    ++checks;
    if (!ok) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

void op_queue_mesh_faces(op_hierarchy_mesh *mesh, float *matrix)
{
    if (calls >= 16) {
        ++failures;
        return;
    }
    got_mesh[calls] = mesh;
    got_matrix[calls] = matrix;
    ++calls;
    if (mutate_on_call == calls)
        mutate_parent->child_count = mutate_count;
    if (switch_on_call == calls) {
        op_hierarchy_thing = &other_thing;
        op_hierarchy_geoset = &other_geoset;
    }
}

static void clear_all(void)
{
    memset(amputated, 0, sizeof amputated);
    memset(hidden, 0, sizeof hidden);
    memset(other_amputated, 0, sizeof other_amputated);
    memset(other_hidden, 0, sizeof other_hidden);
    memset(&thing, 0, sizeof thing);
    memset(&other_thing, 0, sizeof other_thing);
    memset(&geoset, 0, sizeof geoset);
    memset(&other_geoset, 0, sizeof other_geoset);
    thing.matrices = matrices;
    thing.amputated_nodes = amputated;
    thing.hidden_meshes = hidden;
    geoset.meshes = meshes;
    other_thing.matrices = other_matrices;
    other_thing.amputated_nodes = other_amputated;
    other_thing.hidden_meshes = other_hidden;
    other_geoset.meshes = other_meshes;
    op_hierarchy_thing = &thing;
    op_hierarchy_geoset = &geoset;
    calls = 0;
    mutate_parent = 0;
    mutate_count = 0;
    mutate_on_call = 0;
    switch_on_call = 0;
}

static op_hierarchy_node node(int ordinal, int mesh)
{
    op_hierarchy_node result;
    memset(&result, 0, sizeof result);
    result.ordinal = ordinal;
    result.mesh_index = mesh;
    return result;
}

int main(void)
{
    op_hierarchy_node root, a, b, c;
    check(sizeof(op_hierarchy_mesh) == 112 && sizeof matrices[0] == 48,
          "original mesh and matrix strides");
    check(offsetof(op_hierarchy_node, ordinal) == 0x44 &&
          offsetof(op_hierarchy_node, mesh_index) == 0x4c &&
          offsetof(op_hierarchy_node, child_count) == 0x54 &&
          offsetof(op_hierarchy_node, first_child) == 0x58 &&
          offsetof(op_hierarchy_node, next_sibling) == 0x5c,
          "original node field offsets");
    check(offsetof(op_hierarchy_thing_state, matrices) == 0x20 &&
          offsetof(op_hierarchy_thing_state, amputated_nodes) == 0x28 &&
          offsetof(op_hierarchy_thing_state, hidden_meshes) == 0x2c &&
          offsetof(op_hierarchy_geoset_state, meshes) == 4,
          "original current-state field offsets");

    clear_all();
    root = node(3, 2);
    op_queue_model_hierarchy(&root);
    check(calls == 1, "leaf submits");
    check(got_mesh[0] == &meshes[2], "mesh stride");
    check(got_matrix[0] == matrices[3], "matrix ordinal");

    clear_all();
    root = node(0, -1);
    op_queue_model_hierarchy(&root);
    check(calls == 0, "minus-one sentinel");

    clear_all();
    root = node(0, 1);
    a = node(4, 3);
    root.child_count = 1;
    root.first_child = &a;
    hidden[1] = -7;
    op_queue_model_hierarchy(&root);
    check(calls == 1, "any nonzero hidden root keeps child");
    check(got_mesh[0] == &meshes[3] && got_matrix[0] == matrices[4],
          "visible child selection");

    clear_all();
    root = node(0, 0);
    a = node(1, 1);
    b = node(2, 2);
    c = node(3, 3);
    root.child_count = 2;
    root.first_child = &b;
    b.next_sibling = &a;
    b.child_count = 1;
    b.first_child = &c;
    amputated[0] = 1;
    op_queue_model_hierarchy(&root);
    check(calls == 4, "root amputation is not a gate here");
    check(got_mesh[0] == &meshes[0] && got_mesh[1] == &meshes[2] &&
          got_mesh[2] == &meshes[3] && got_mesh[3] == &meshes[1],
          "preorder follows sibling chain, not ordinal order");
    calls = 0;
    amputated[2] = -3;
    op_queue_model_hierarchy(&root);
    check(calls == 2, "any nonzero amputation skips entire subtree");
    check(got_mesh[0] == &meshes[0] && got_mesh[1] == &meshes[1],
          "root before remaining sibling");

    clear_all();
    root = node(0, 0);
    a = node(1, 1);
    b = node(2, 2);
    root.child_count = 2;
    root.first_child = &a;
    a.next_sibling = &b;
    mutate_parent = &root;
    mutate_count = 1;
    mutate_on_call = 2;
    op_queue_model_hierarchy(&root);
    check(calls == 2, "updated parent count shortens traversal");
    calls = 0;
    root.child_count = 1;
    mutate_count = 2;
    op_queue_model_hierarchy(&root);
    check(calls == 3 && got_mesh[2] == &meshes[2],
          "updated parent count extends valid sibling traversal");

    clear_all();
    root = node(0, 0);
    a = node(1, 1);
    b = node(2, 2);
    root.child_count = 2;
    root.first_child = &a;
    a.next_sibling = &b;
    other_amputated[1] = 1;
    switch_on_call = 1;
    op_queue_model_hierarchy(&root);
    check(calls == 2, "callback-selected state controls subsequent child gates");
    check(got_mesh[0] == &meshes[0] && got_matrix[0] == matrices[0] &&
          got_mesh[1] == &other_meshes[2] && got_matrix[1] == other_matrices[2],
          "callback-selected geoset and matrix array used by remaining child");

    printf("Model hierarchy: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
