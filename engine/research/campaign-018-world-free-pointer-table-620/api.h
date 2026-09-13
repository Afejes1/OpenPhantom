#ifndef OP_CAMPAIGN_018_WORLD_FREE_POINTER_TABLE_620_API_H
#define OP_CAMPAIGN_018_WORLD_FREE_POINTER_TABLE_620_API_H
/* Research partial view with four fixture-accessible slots, NOT a recovered
 * original capacity. Only offsets620/624 are established here. A canonical
 * integration must reconcile full storage before claiming larger C domains. */
typedef struct OP_WORLD {
    unsigned char before_count[0x620];
    int count;
    void *entries[4];
} OP_WORLD;
void op_release(void *memory);
void op_world_free_pointer_table_620(OP_WORLD *world);
#endif
