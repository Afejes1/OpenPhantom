#ifndef OP_CAMPAIGN059_API_H
#define OP_CAMPAIGN059_API_H
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
