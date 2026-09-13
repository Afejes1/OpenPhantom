#ifndef OP_CAMPAIGN_014_SOUND_MODULE_PROC_API_H
#define OP_CAMPAIGN_014_SOUND_MODULE_PROC_API_H

extern int op_sound_performance_level;
extern int op_sound_quality_level;

int op_sound_module_init(void);
int op_sound_shutdown(void);
int op_sound_register_level(void);
int op_sound_remove_level(int ignored);
void op_sound_reset_places(void);
void op_sound_stop_all(void);
int op_sound_periodic(void);
void op_sound_pause_all(void);
void op_sound_resume_all(void);
int op_sound_module_proc(int event);

#endif
