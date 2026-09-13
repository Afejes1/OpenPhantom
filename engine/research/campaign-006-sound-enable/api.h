#ifndef OP_CAMPAIGN_006_SOUND_ENABLE_API_H
#define OP_CAMPAIGN_006_SOUND_ENABLE_API_H
extern int op_sound_disabled;
extern const char op_sound_disabled_key[];
int op_sound_write_option(const char *key,int value);
void op_sound_enable(void);
#endif