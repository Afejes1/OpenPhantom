#ifndef OP_APPLICATION_HELPERS_H
#define OP_APPLICATION_HELPERS_H
extern int op_application_mode, op_simulation_halted, op_diagnostics_enabled;
extern void *op_diagnostic_stream;
extern unsigned int op_projection_counter;
extern float op_effect_seconds;
extern const float op_flicker_frequency, op_flicker_one, op_flicker_quarter, op_flicker_half;
int op_initialize_systems(void);
void op_shutdown_systems(void);
void op_run_campaign(void);
void op_run_other_branch(void);
void op_clear_graphics_state_a(void);
void op_clear_graphics_state_b(void);
int op_close_graphics_device(void);
int op_shutdown_graphics(void);
int op_close_diagnostic_stream(void *);
float op_sine_degrees(float);
int op_application_run(void);
void op_graphics_close(void);
void op_diagnostics_close(void);
void op_simulation_set_halted(int);
float op_halo_flicker(void);
unsigned int op_allocate_projection_token(void);
int op_overlay_save_size(void);
#endif
