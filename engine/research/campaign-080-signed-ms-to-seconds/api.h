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
#endif
