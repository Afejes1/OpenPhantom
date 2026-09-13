#ifndef OP_CAMPAIGN_013_MUSIC_SET_SEQUENCE_API_H
#define OP_CAMPAIGN_013_MUSIC_SET_SEQUENCE_API_H

typedef void (__stdcall *op_music_callback)(void);

extern int op_music_initialized;
extern int op_music_cached_sequence;
extern int op_music_lock_depth;
extern op_music_callback op_music_lock;
extern op_music_callback op_music_unlock;

void *op_effective_player(void);
unsigned int op_count_nearby(void *player, float radius);
int op_backend_sequence(int sequence);
int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius);

#endif
