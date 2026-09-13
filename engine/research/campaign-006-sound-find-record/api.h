#ifndef OP_CAMPAIGN_006_SOUND_FIND_RECORD_API_H
#define OP_CAMPAIGN_006_SOUND_FIND_RECORD_API_H
extern void *op_sound_registry;
void *op_sound_lookup(void *registry, const char *name);
void *op_sound_find_record(const char *name);
#endif