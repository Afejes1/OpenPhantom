#ifndef OP_CAMPAIGN_018_WORLD_FREE_POINTER_TABLE_220_API_H
#define OP_CAMPAIGN_018_WORLD_FREE_POINTER_TABLE_220_API_H
/* Partial view:255 entries reach the independently observed next count at620.
 * This is an inferred view extent, not recovered original capacity validation. */
typedef struct OP_WORLD {
    unsigned char before_count[0x220];
    int count;
    void *entries[255];
} OP_WORLD;
void op_release(void *memory);
void op_world_free_pointer_table_220(OP_WORLD *world);
#endif
