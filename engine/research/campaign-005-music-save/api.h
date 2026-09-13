#ifndef OP_CAMPAIGN_005_MUSIC_SAVE_API_H
#define OP_CAMPAIGN_005_MUSIC_SAVE_API_H

extern int op_music_state;
extern int op_music_sequence;
extern int op_music_cached_ids[2];
int op_music_write_frame(int tag, const void *data, int size,
                         unsigned short version);
int op_music_save_cached_ids(int tag);

#endif