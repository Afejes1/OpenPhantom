#ifndef OP_CAMPAIGN_005_MUSIC_PAUSE_API_H
#define OP_CAMPAIGN_005_MUSIC_PAUSE_API_H

typedef void (__stdcall *op_music_lock_fn)(void);
extern op_music_lock_fn op_music_lock;
extern op_music_lock_fn op_music_unlock;
extern int op_music_lock_depth;
extern int op_music_paused;
void op_music_pause_backend(void);
void op_music_pause(void);

#endif