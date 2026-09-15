#ifndef OP_FRAME_PACING_H
#define OP_FRAME_PACING_H
#include "elapsed_time.h"
#include "platform_helpers.h"
extern float op_skills_frame_delta, op_frame_elapsed;
extern unsigned int op_fixed_rate_gate;
extern int op_frame_wait_enabled;
extern void(__stdcall *op_sleep)(unsigned int);
unsigned int op_pump_messages(void);
void op_pace_frame_clock(void);
#endif
