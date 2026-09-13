#ifndef OP_CAMPAIGN_009_SOUND_START_SAMPLE_API_H
#define OP_CAMPAIGN_009_SOUND_START_SAMPLE_API_H
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int type;
    unsigned char opaque_0c[4];
    unsigned int flags;
    unsigned char rest[108];
} op_sound_channel;
typedef void (__stdcall *op_sound_loop_fn)(void *sample, int loops);
typedef void (__stdcall *op_sound_start_fn)(void *sample);
extern op_sound_channel op_sound_channels[12];
extern op_sound_loop_fn op_sound_loop2d;
extern op_sound_loop_fn op_sound_loop3d;
extern op_sound_start_fn op_sound_start2d;
extern op_sound_start_fn op_sound_start3d;
void op_sound_start_sample(unsigned int index);
#endif
