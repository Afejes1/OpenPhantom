#ifndef OP_CAMPAIGN_008_SOUND_PERIODIC_API_H
#define OP_CAMPAIGN_008_SOUND_PERIODIC_API_H
extern int op_sound_initialized;
extern int op_sound_mode;
extern int op_sound_world_enabled;
extern int op_sound_paused;
void op_sound_update_listener(void);
void op_sound_update_placements(void);
void op_sound_poll_channel(int index);
int op_sound_periodic(void);
#endif
