#ifndef OP_CAMPAIGN_005_MUSIC_ENABLE_API_H
#define OP_CAMPAIGN_005_MUSIC_ENABLE_API_H

extern const char op_music_disabled_key[];
extern int op_music_resume_state;
extern int op_music_resume_sequence;
int op_music_write_option(const char *key, int value);
int op_music_initialize(void);
int op_music_set_state(int state);
int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius);
void op_music_enable(void);

#endif