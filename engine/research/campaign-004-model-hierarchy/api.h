#ifndef OP_CAMPAIGN_004_MODEL_HIERARCHY_API_H
#define OP_CAMPAIGN_004_MODEL_HIERARCHY_API_H

typedef struct op_hierarchy_node {
    unsigned char opaque_00[0x44];
    int ordinal;
    unsigned char opaque_48[4];
    int mesh_index;
    unsigned char opaque_50[4];
    unsigned int child_count;
    struct op_hierarchy_node *first_child;
    struct op_hierarchy_node *next_sibling;
} op_hierarchy_node;

typedef struct op_hierarchy_thing_state {
    unsigned char opaque_00[0x20];
    float (*matrices)[12];
    unsigned char opaque_24[4];
    int *amputated_nodes;
    int *hidden_meshes;
} op_hierarchy_thing_state;

typedef struct op_hierarchy_mesh { unsigned char bytes[112]; } op_hierarchy_mesh;

typedef struct op_hierarchy_geoset_state {
    unsigned char opaque_00[4];
    op_hierarchy_mesh *meshes;
} op_hierarchy_geoset_state;

extern op_hierarchy_thing_state *op_hierarchy_thing;
extern op_hierarchy_geoset_state *op_hierarchy_geoset;
void op_queue_mesh_faces(op_hierarchy_mesh *mesh, float *matrix);
void op_queue_model_hierarchy(op_hierarchy_node *node);
#endif