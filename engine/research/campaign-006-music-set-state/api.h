#ifndef OP_CAMPAIGN_006_MUSIC_SET_STATE_API_H
#define OP_CAMPAIGN_006_MUSIC_SET_STATE_API_H

typedef void (__stdcall *op_music_lock_fn)(void);
extern op_music_lock_fn op_music_lock;
extern op_music_lock_fn op_music_unlock;
extern int op_music_initialized;
extern int op_music_state;
extern int op_music_lock_depth;
int op_music_backend_set_state(int state);
int op_music_set_state(int requested);

#endif