#ifndef OP_ELAPSED_TIME_H
#define OP_ELAPSED_TIME_H
extern unsigned int op_time_start_ticks, op_time_previous_ticks;
extern float op_time_start_seconds;
extern const float op_time_millisecond_unit;
int op_platform_ticks(void);
float op_platform_seconds(void);
unsigned int op_elapsed_ticks(void);
float op_elapsed_seconds(void);
unsigned int op_delta_ticks(void);
float op_signed_ms_to_seconds(int);
extern int op_time_mode;
extern float op_time_previous_seconds;
extern const float op_time_thousand;
int op_time_module_proc(int);
#endif

#ifndef OP_PLATFORM_HELPERS_H
#define OP_PLATFORM_HELPERS_H
void *op_platform_heap_alloc(unsigned int);
void op_platform_heap_free(void *);
void *op_platform_heap_realloc(void *, unsigned int);
void *op_platform_alloc_debug(unsigned int);
void op_platform_free_debug(void *);
void *op_platform_realloc_debug(void *, unsigned int);
void *op_platform_lock_identity(void *);
extern int op_platform_highres;
extern double op_platform_millisecond_scale, op_platform_second_scale;
extern unsigned int(__stdcall *op_platform_time_get_time)(void);
extern int(__stdcall *op_platform_query_counter)(__int64 *);
int op_platform_ticks(void);
float op_platform_seconds(void);
#endif
