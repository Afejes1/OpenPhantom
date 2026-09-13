#ifndef OP_CAMPAIGN_007_SOUND_RELEASE_HANDLE_API_H
#define OP_CAMPAIGN_007_SOUND_RELEASE_HANDLE_API_H
typedef struct op_vec3 { float x, y, z; } op_vec3;
typedef struct op_sound_channel { void *sample; void *sample3d; int is3d; int active; unsigned int flags; unsigned char opaque_14[0x40]; op_vec3 position; unsigned char opaque_60[0x20]; } op_sound_channel;
typedef void (__stdcall *op_sound_release_fn)(void *handle);
extern op_sound_channel op_sound_channels[12];
extern op_sound_release_fn op_sound_release_2d;
extern op_sound_release_fn op_sound_release_3d;
void op_sound_release_handle(int index);
#endif