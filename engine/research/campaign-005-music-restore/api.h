#ifndef OP_CAMPAIGN_005_MUSIC_RESTORE_API_H
#define OP_CAMPAIGN_005_MUSIC_RESTORE_API_H

extern int op_music_cached_ids[2];
int op_music_read_bytes(void *data, int size);
int op_music_restore_cached_ids(int version);

#endif