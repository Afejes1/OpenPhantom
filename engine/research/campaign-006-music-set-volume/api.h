#ifndef OP_CAMPAIGN_006_MUSIC_SET_VOLUME_API_H
#define OP_CAMPAIGN_006_MUSIC_SET_VOLUME_API_H

typedef void (__stdcall *op_music_lock_fn)(void);
extern op_music_lock_fn op_music_lock;
extern op_music_lock_fn op_music_unlock;
extern float op_music_volume;
extern int op_music_initialized;
extern int op_music_lock_depth;
void *op_music_grab_buffer(void);
void op_music_apply_volume(void *buffer, float volume);
void op_music_set_volume(float volume);

#endif