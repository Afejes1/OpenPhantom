#ifndef OP_CAMPAIGN_010_SOUND_FREE_CHANNEL_API_H
#define OP_CAMPAIGN_010_SOUND_FREE_CHANNEL_API_H
typedef struct op_sound_record {
    unsigned char opaque[0x38];
    void *resource;
} op_sound_record;
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int type;
    op_sound_record *record;
    unsigned int flags;
    unsigned char opaque14[0x2c];
    unsigned int state40;
    unsigned char opaque44[0x2c];
    unsigned int state70;
    unsigned char opaque74[4];
    int *caller_handle;
    float *position_owner;
} op_sound_channel;
extern op_sound_channel op_sound_channels[12];
extern void (__stdcall *op_sound_end2d)(void *sample);
extern void (__stdcall *op_sound_end3d)(void *sample);
int op_sound_mark_resource(void *resource, int flag);
int op_sound_release_resource(void *resource);
void op_sound_free_channel(unsigned int slot);
#endif
