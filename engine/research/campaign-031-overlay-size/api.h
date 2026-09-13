#ifndef OP_CAMPAIGN031_H
#define OP_CAMPAIGN031_H
#include <stddef.h>
typedef struct OP_VEC3
{
    float x, y, z;
} OP_VEC3;
typedef struct OP_RIPPLE
{
    int active, moving, appearance;
    OP_VEC3 position, velocity;
    float yaw, radius, radial_velocity, render_parameter, lifetime, time, last_update;
    int token, required_type;
} OP_RIPPLE;
typedef char op_layout[(sizeof(void *) == 4 && sizeof(OP_VEC3) == 12 && sizeof(OP_RIPPLE) == 72 &&
                        offsetof(OP_RIPPLE, last_update) == 60 && offsetof(OP_RIPPLE, token) == 64)
                           ? 1
                           : -1];
extern OP_RIPPLE op_ripples[256];
extern OP_VEC3 op_zap_points[];
extern unsigned int op_projection_counter;
extern float op_effect_seconds;
extern const float op_float_zero, op_float_one, op_flicker_frequency, op_flicker_one, op_flicker_quarter,
    op_flicker_half;
extern const float op_zap_half, op_random_scale, op_zap_decay;
float op_sine_degrees(float);
int op_random(void);
void op_remove_projection_group(int);
void op_render_ripple(OP_VEC3 *, float, float, int, float, int, float, int, int);
int op_overlay_size(void);
unsigned int op_projection_token(void);
float op_halo_flicker(void);
void op_ripple_tick(void);
void op_zap_subdivide(int, int, int, float);
#endif
