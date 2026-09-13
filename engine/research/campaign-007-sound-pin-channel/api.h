#ifndef OP_CAMPAIGN_007_SOUND_PIN_CHANNEL_API_H
#define OP_CAMPAIGN_007_SOUND_PIN_CHANNEL_API_H
typedef struct op_vec3 { float x, y, z; } op_vec3;
typedef struct op_sound_channel {
    void *sample;
    void *sample3d;
    int is3d;
    int active;
    unsigned int flags;
    unsigned char opaque_14[0x40];
    op_vec3 position;
    unsigned char opaque_60[0x20];
} op_sound_channel;
extern op_sound_channel op_sound_channels[12];
void op_sound_pin_channel(int index, const op_vec3 *position);
#endif