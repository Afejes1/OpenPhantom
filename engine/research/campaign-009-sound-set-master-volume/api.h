#ifndef OP_CAMPAIGN_009_SOUND_SET_MASTER_VOLUME_API_H
#define OP_CAMPAIGN_009_SOUND_SET_MASTER_VOLUME_API_H
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int type;
    unsigned char opaque_0c[12];
    float volume;
    unsigned char rest[100];
} op_sound_channel;
typedef void (__stdcall *op_sound_volume_fn)(void *handle, int volume);
extern int op_sound_initialized;
extern float op_sound_master_gain;
extern void *op_sound_driver;
extern op_sound_channel op_sound_channels[12];
extern op_sound_volume_fn op_sound_set_volume3d;
extern op_sound_volume_fn op_sound_set_master;
void op_sound_set_master_volume(int value);
#endif
