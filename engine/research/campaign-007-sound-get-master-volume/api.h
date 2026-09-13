#ifndef OP_CAMPAIGN_007_SOUND_GET_MASTER_VOLUME_API_H
#define OP_CAMPAIGN_007_SOUND_GET_MASTER_VOLUME_API_H
typedef int (__stdcall *op_sound_query_volume_fn)(void *driver);
extern op_sound_query_volume_fn op_sound_query_volume;
extern int op_sound_initialized;
extern void *op_sound_driver;
int op_sound_get_master_volume(void);
#endif