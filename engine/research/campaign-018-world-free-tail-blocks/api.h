#ifndef OP_CAMPAIGN_018_WORLD_FREE_TAIL_BLOCKS_API_H
#define OP_CAMPAIGN_018_WORLD_FREE_TAIL_BLOCKS_API_H
typedef struct OP_WORLD {
    unsigned char before_sprite_count[0xcb4];
    int sprite_count;
    void *sprites;
    unsigned int sound_placement_count;
    void *sound_placements;
    unsigned int sound_descriptor_count;
    void *sound_descriptors;
} OP_WORLD;
void op_release(void *memory);
void op_world_free_tail_blocks(OP_WORLD *world);
#endif
