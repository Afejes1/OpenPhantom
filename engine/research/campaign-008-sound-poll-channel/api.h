#ifndef OP_CAMPAIGN_008_SOUND_POLL_CHANNEL_API_H
#define OP_CAMPAIGN_008_SOUND_POLL_CHANNEL_API_H
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int is3d;
    void *record;
    unsigned int flags;
    unsigned char opaque_14[0x6c];
} op_sound_channel;
typedef int (__stdcall *op_sound_status_fn)(void *sample);
extern op_sound_channel op_sound_channels[12];
extern op_sound_status_fn op_sound_status2d;
extern op_sound_status_fn op_sound_status3d;
void op_sound_release_slot(unsigned int index);
int op_sound_update_slot(unsigned int index, int mode);
void op_sound_poll_channel(unsigned int index);
#endif
