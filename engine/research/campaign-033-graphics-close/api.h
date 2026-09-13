#ifndef OP_CAMPAIGN033_H
#define OP_CAMPAIGN033_H
#include <stddef.h>
typedef char op_abi[(sizeof(void *) == 4 && sizeof(int) == 4) ? 1 : -1];
extern int op_application_mode, op_simulation_halted, op_diagnostics_enabled;
extern void *op_diagnostic_stream, *op_active_world;
extern unsigned char *op_camera_packet;
int op_initialize_systems(void);
void op_shutdown_systems(void);
void op_run_campaign(void);
void op_run_other_branch(void);
void op_prepare_world(void *);
void op_traverse_world(void *);
void op_prepare_world_surfaces(void *, void *, int);
void op_prepare_graphics_submission(void);
void op_submit_world_surfaces(void);
void op_clear_graphics_state_a(void);
void op_clear_graphics_state_b(void);
int op_close_graphics_device(void);
int op_shutdown_graphics(void);
int op_close_diagnostic_stream(void *);
int op_application_run(void);
void op_frame_prepare(void);
void op_graphics_close(void);
void op_diagnostics_close(void);
void op_simulation_set_halted(int);
#endif
