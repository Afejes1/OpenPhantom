#ifndef OP_CAMPAIGN_018_WORLD_FREE_MATERIALS_API_H
#define OP_CAMPAIGN_018_WORLD_FREE_MATERIALS_API_H
/* Partial views; neighboring offsets give 32 slots between starts.
 * This does not establish complete original capacity validation. */
typedef struct OP_WORLD {
    int load_state;
    unsigned char before_count[0xb8];
    int texture_count;
    unsigned char before_primary[4];
    void *primary[32];
    void *secondary[32];
} OP_WORLD;
void op_destroy_material(void *material);
void op_world_free_materials(OP_WORLD *world);
#endif
