#ifndef OP_CAMPAIGN_005_MUSIC_PERIODIC_API_H
#define OP_CAMPAIGN_005_MUSIC_PERIODIC_API_H

typedef void (__stdcall *op_music_lock_fn)(void);
extern op_music_lock_fn op_music_lock;
extern op_music_lock_fn op_music_unlock;
extern int op_music_initialized;
extern int op_music_lock_depth;
void op_music_service(void);
void op_music_periodic(void);

#endif