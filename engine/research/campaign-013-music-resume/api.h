#ifndef OP_CAMPAIGN_013_MUSIC_RESUME_API_H
#define OP_CAMPAIGN_013_MUSIC_RESUME_API_H

typedef void (__stdcall *op_music_callback)(void);

extern op_music_callback op_music_lock;
extern op_music_callback op_music_unlock;
extern int op_music_lock_depth;
extern int op_music_paused;

void op_backend_resume(void);
void op_music_resume(void);

#endif
