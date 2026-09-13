#ifndef OP_CAMPAIGN_006_MUSIC_DETACH_API_H
#define OP_CAMPAIGN_006_MUSIC_DETACH_API_H

typedef void (__stdcall *op_music_lock_fn)(void);
typedef void (__cdecl *op_music_shutdown_fn)(void);
extern op_music_lock_fn op_music_lock;
extern op_music_lock_fn op_music_unlock;
extern op_music_shutdown_fn op_music_shutdown;
extern int op_music_initialized;
extern int op_music_paused;
extern int op_music_lock_depth;
void op_music_resume(void);
int op_music_set_state(int state);
int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius);
void op_music_stop_all(void);
void op_music_terminate_script(void);
void op_music_detach(void);

#endif