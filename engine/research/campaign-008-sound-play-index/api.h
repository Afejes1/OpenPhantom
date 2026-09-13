#ifndef OP_CAMPAIGN_008_SOUND_PLAY_INDEX_API_H
#define OP_CAMPAIGN_008_SOUND_PLAY_INDEX_API_H
typedef struct op_sound_descriptor { unsigned char bytes[64]; } op_sound_descriptor;
typedef struct op_sound_world_state {
    unsigned char opaque_000[0xcc4];
    unsigned int descriptor_count;
    op_sound_descriptor *descriptors;
} op_sound_world_state;
extern op_sound_world_state *op_sound_world;
extern int op_sound_mode;
int op_sound_start_descriptor(op_sound_descriptor *descriptor, int *handle,
                              float *position);
void op_sound_play_index(unsigned int index, int *handle, float *position);
#endif
