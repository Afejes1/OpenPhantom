#ifndef OP_CAMPAIGN030_API_H
#define OP_CAMPAIGN030_API_H
#include <stddef.h>
typedef struct OP_VEC3
{
    float x, y, z;
} OP_VEC3;
typedef struct OP_HIT
{
    int active, state;
    OP_VEC3 direction, basis;
    float progress, lifetime;
    int mode;
} OP_HIT;
typedef struct OP_SHIELD
{
    int active, visible, stopping, no_save;
    void *actor;
    float elapsed;
    unsigned char before_color[32];
    unsigned char red, green, blue, alpha;
    unsigned char before_hits[8];
    unsigned int hit_count;
    OP_HIT *hits;
    float radius;
    unsigned char before_name[16];
    char name[80];
    int saved_actor;
} OP_SHIELD;
typedef struct OP_SHIELD_SAVE
{
    int slot, visible;
    float elapsed;
    char name[32];
    float radius;
    unsigned int color;
} OP_SHIELD_SAVE;
typedef struct OP_RIPPLE
{
    int active, moving, appearance;
    OP_VEC3 position, velocity;
    float yaw, radius, radial_velocity, render_parameter, lifetime, time;
    int state, token, required_type;
} OP_RIPPLE;
typedef char op_layout[(sizeof(void *) == 4 && sizeof(OP_VEC3) == 12 && sizeof(OP_HIT) == 44 &&
                        sizeof(OP_SHIELD) == 180 && offsetof(OP_SHIELD, hit_count) == 68 &&
                        offsetof(OP_SHIELD, hits) == 72 && offsetof(OP_SHIELD, saved_actor) == 176 &&
                        sizeof(OP_SHIELD_SAVE) == 52 && sizeof(OP_RIPPLE) == 72 && offsetof(OP_RIPPLE, token) == 64)
                           ? 1
                           : -1];
extern OP_SHIELD op_shields[32];
extern OP_RIPPLE op_ripples[256];
extern OP_VEC3 op_zap_points[];
extern void *op_zap_sprite;
extern int op_ripples_enabled, op_projection_enabled;
extern float op_effect_seconds;
extern const float op_float_zero, op_degrees_per_turn, op_zap_amplitude_scale;
int op_save_read(void *, unsigned int);
int op_shield_allocate(void *);
void op_shield_set_texture(int, char *);
void op_subdivide_zap(int, int, int, float);
void op_generate_zap_points(OP_VEC3 *, OP_VEC3 *, int);
void *op_resolve_zap_material(void *);
void op_submit_zap_points(OP_VEC3 *, int, unsigned int, void *, float);
int op_allocate_projection_token(void);
float op_normalize_vector(OP_VEC3 *);
void op_reference_axis(OP_VEC3 *, OP_VEC3 *);
void op_identity_transform(void);
void op_rotate_transform(OP_VEC3 *, float);
void op_transform_point(OP_VEC3 *);
int op_shield_load(void);
void op_zap_build(OP_VEC3 *, OP_VEC3 *, int);
void op_zap_draw_opaque(OP_VEC3 *, OP_VEC3 *, OP_VEC3 *, int, float);
int op_ripple_spawn(int, OP_VEC3 *, OP_VEC3 *, float, float, float, float, float, int);
OP_HIT *op_shield_hit(int, OP_VEC3 *, float);
#endif
