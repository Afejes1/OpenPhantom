#ifndef OP_CAMPAIGN_006_SOUND_DISABLE_API_H
#define OP_CAMPAIGN_006_SOUND_DISABLE_API_H
extern int op_sound_disabled;
extern const char op_sound_disabled_key[];
int op_sound_write_option(const char *key,int value);
void op_sound_stop_all(void);
void op_sound_disable(void);
#endif