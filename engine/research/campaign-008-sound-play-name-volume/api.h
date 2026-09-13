#ifndef OP_CAMPAIGN_008_SOUND_PLAY_NAME_VOLUME_API_H
#define OP_CAMPAIGN_008_SOUND_PLAY_NAME_VOLUME_API_H
extern int op_sound_initialized;
extern int op_sound_mode;
void op_sound_set_field(int field, float value);
int op_sound_play_descriptor_name(int slot, const char *name, int *handle,
                                  float *position, unsigned int flags);
int op_sound_play_name_volume(const char *name, float volume,
                              unsigned int flags);
#endif
