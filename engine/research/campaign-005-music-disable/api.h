#ifndef OP_CAMPAIGN_005_MUSIC_DISABLE_API_H
#define OP_CAMPAIGN_005_MUSIC_DISABLE_API_H

extern const char op_music_disabled_key[];
extern int op_music_state;
extern int op_music_sequence;
extern int op_music_resume_state;
extern int op_music_resume_sequence;
int op_music_write_option(const char *key, int value);
void op_music_detach(void);
void op_music_disable(void);

#endif