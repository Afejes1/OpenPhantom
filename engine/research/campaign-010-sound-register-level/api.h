#ifndef OP_CAMPAIGN_010_SOUND_REGISTER_LEVEL_API_H
#define OP_CAMPAIGN_010_SOUND_REGISTER_LEVEL_API_H
typedef struct op_sound_level_record { unsigned int flags; char name[52]; void *resource; } op_sound_level_record;
typedef struct op_sound_world_state { unsigned char opaque_000[0xcb4]; int record_count; op_sound_level_record *records; } op_sound_world_state;
extern int op_sound_initialized,op_sound_load_failures,op_sound_category,op_sound_ref_count,op_sound_level_registered;
extern op_sound_world_state *op_sound_world; extern void *op_sound_registry;
void op_sound_init_channels(void); void op_sound_bind_listener(float*,float*,float*);
int op_sound_insert_name(void*,const char*,op_sound_level_record*); int op_sound_load_ref(op_sound_level_record*); int op_sound_remove_name(void*,const char*);
int op_sound_register_level(void);
#endif
