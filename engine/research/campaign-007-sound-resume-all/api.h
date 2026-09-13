#ifndef OP_CAMPAIGN_007_SOUND_RESUME_ALL_API_H
#define OP_CAMPAIGN_007_SOUND_RESUME_ALL_API_H
typedef struct op_sound_channel { void *sample; void *sample3d; int is3d; int active; unsigned int flags; unsigned char opaque_14[0x6c]; } op_sound_channel;
typedef void (__stdcall *op_sound_resume_fn)(void *handle);
extern op_sound_channel op_sound_channels[12];
extern op_sound_resume_fn op_sound_resume_2d;
extern op_sound_resume_fn op_sound_resume_3d;
void op_sound_resume_all(void);
#endif