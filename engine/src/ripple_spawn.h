#ifndef OP_RIPPLE_SPAWN_H
#define OP_RIPPLE_SPAWN_H
#include <stddef.h>
typedef struct OP_VEC3
{
    float x, y, z;
} OP_VEC3;
typedef struct OP_RIPPLE
{
    int active, moving, appearance;
    OP_VEC3 position, velocity;
    float yaw, radius, radial_velocity, render_parameter, lifetime, time;
    int state, token, required_type;
} OP_RIPPLE;
typedef char ripple_layout[(sizeof(void *) == 4 && sizeof(OP_VEC3) == 12 && sizeof(OP_RIPPLE) == 72 &&
                            offsetof(OP_RIPPLE, token) == 64)
                               ? 1
                               : -1];
extern OP_RIPPLE op_ripples[256];
extern int op_ripples_enabled, op_projection_enabled;
extern float op_effect_seconds;
extern const float op_float_zero;
int op_allocate_projection_token(void);
int op_ripple_spawn(int, OP_VEC3 *, OP_VEC3 *, float, float, float, float, float, int);
typedef char ripple_offsets[(offsetof(OP_RIPPLE, position) == 12 && offsetof(OP_RIPPLE, velocity) == 24 &&
                             offsetof(OP_RIPPLE, yaw) == 36 && offsetof(OP_RIPPLE, time) == 56 &&
                             offsetof(OP_RIPPLE, required_type) == 68)
                                ? 1
                                : -1];
#endif
