#ifndef OP_CAMPAIGN_010_SOUND_MODULE_INIT_API_H
#define OP_CAMPAIGN_010_SOUND_MODULE_INIT_API_H
typedef int (__stdcall *op_sound_quick_startup_fn)(int,int,int,int,int);
typedef int (__stdcall *op_sound_preference_fn)(int,int);
typedef void (__stdcall *op_sound_quick_handles_fn)(void **,void **,void **);
extern int op_sound_initialized;
extern void *op_sound_driver, *op_sound_registry;
extern const char op_sound_volume_key[], op_sound_disabled_key[];
extern op_sound_quick_startup_fn op_sound_quick_startup;
extern op_sound_preference_fn op_sound_preference;
extern op_sound_quick_handles_fn op_sound_quick_handles;
void *op_sound_create_registry(int capacity);
void op_sound_init3d(void); void op_sound_init_channels(void);
void op_sound_set_master_volume(int value); void op_sound_disable(void); void op_sound_enable(void);
int op_sound_read_option(const char *key,int fallback,int *out);
int op_sound_module_init(void);
#endif
