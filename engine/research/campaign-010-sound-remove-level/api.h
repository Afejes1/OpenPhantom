#ifndef OP_CAMPAIGN_010_SOUND_REMOVE_LEVEL_API_H
#define OP_CAMPAIGN_010_SOUND_REMOVE_LEVEL_API_H
typedef struct op_sound_level_record { unsigned int flags; char name[52]; void *resource; } op_sound_level_record;
typedef struct op_sound_world_state { unsigned char opaque_000[0xcb4]; int record_count; op_sound_level_record *records; } op_sound_world_state;
extern int op_sound_initialized, op_sound_ref_count, op_sound_level_registered;
extern op_sound_world_state *op_sound_world;
extern void *op_sound_registry;
void op_sound_resume(void);
void op_sound_stop_all(void);
void op_sound_free_extra(void);
int op_sound_mark_resource(void *resource, int flag);
int op_sound_release_resource(void *resource);
int op_sound_remove_name(void *registry, const char *name);
int op_sound_remove_level(int ignored);
#endif
