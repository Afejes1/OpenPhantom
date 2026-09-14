#ifndef OP_FIXED_STEPS_API_H
#define OP_FIXED_STEPS_API_H
#include "task_manager.h"
#include "b3d_chunk_readers.h"
#include "module.h"
extern float op_skills_frame_delta, op_time_mode;
extern unsigned int op_fixed_rate_gate, op_fixed_generation;
extern const float op_fixed_max_delta;
extern OP_B3D_WORLD *op_active_world;
void op_advance_runtime_fixed_steps(unsigned int scheduler_word);
#endif
