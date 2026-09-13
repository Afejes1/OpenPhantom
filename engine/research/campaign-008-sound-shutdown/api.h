#ifndef OP_CAMPAIGN_008_SOUND_SHUTDOWN_API_H
#define OP_CAMPAIGN_008_SOUND_SHUTDOWN_API_H

typedef void (__stdcall *op_sound_shutdown_fn)(void);

extern int op_sound_initialized;
extern void *op_sound_registry;
extern op_sound_shutdown_fn op_sound_quick_shutdown;

void op_sound_resume_all(void);
void op_sound_stop_all(void);
void op_sound_free_extra_refs(void);
void op_sound_release_handle(int index);
void op_sound_clear_backend(void);
void op_sound_destroy_registry(void *registry);
int op_sound_shutdown(void);

#endif
