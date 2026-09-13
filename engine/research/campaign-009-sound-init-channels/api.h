#ifndef OP_CAMPAIGN_009_SOUND_INIT_CHANNELS_API_H
#define OP_CAMPAIGN_009_SOUND_INIT_CHANNELS_API_H
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int type;
    void *record;
    unsigned int flags;
    int slot;
    unsigned char rest[104];
} op_sound_channel;
typedef void *(__stdcall *op_sound_allocate_fn)(void *driver);
typedef void (__stdcall *op_sound_init_fn)(void *sample);
extern op_sound_channel op_sound_channels[12];
extern void *op_sound_driver;
extern op_sound_allocate_fn op_sound_allocate_sample;
extern op_sound_init_fn op_sound_init_sample;
void op_sound_release_slot(int index);
void op_sound_release_handle(int index);
void op_sound_init_channels(void);
#endif
