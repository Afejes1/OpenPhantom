#ifndef OP_CAMPAIGN_009_SOUND_PLAY_VOICE_API_H
#define OP_CAMPAIGN_009_SOUND_PLAY_VOICE_API_H
extern int op_sound_mode;
void op_sound_release_slot(unsigned int index);
void op_sound_set_field(int field, float value);
int op_sound_has_input_lock(int minimum);
int op_sound_play_descriptor_name(int slot, const char *name, int *handle,
                                  float *position, unsigned int flags);
void op_sound_play_voice(const char *name, int *handle, float *position);
#endif
