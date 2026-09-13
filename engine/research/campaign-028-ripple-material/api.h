#ifndef OP_CAMPAIGN028_API_H
#define OP_CAMPAIGN028_API_H
#include <stddef.h>
typedef struct OP_ACTOR
{
    unsigned int flags;
    unsigned char rest[0x100];
} OP_ACTOR;
typedef struct OP_HALO
{
    OP_ACTOR *owner;
    unsigned char before_sprite[16];
    void *sprite;
    unsigned char tail[4];
} OP_HALO;
typedef struct OP_EFFECTS_SAVE
{
    unsigned char prefix[24];
    float duration, target, cached, remaining;
    int restore;
    unsigned int saved;
    unsigned char tail[48];
} OP_EFFECTS_SAVE;
typedef struct OP_RIPPLE_MATERIAL
{
    char *color_name, *alpha_name;
    void *material;
} OP_RIPPLE_MATERIAL;
typedef struct OP_RIPPLE
{
    int active;
    unsigned char rest[68];
} OP_RIPPLE;
typedef char op_word_size[sizeof(int) == 4 && sizeof(void *) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef char op_actor_flags[offsetof(OP_ACTOR, flags) == 0 ? 1 : -1];
typedef char op_halo_stride[sizeof(OP_HALO) == 28 && offsetof(OP_HALO, sprite) == 20 ? 1 : -1];
typedef char op_save_size[sizeof(OP_EFFECTS_SAVE) == 96 ? 1 : -1];
typedef char op_save_fields[offsetof(OP_EFFECTS_SAVE, duration) == 24 && offsetof(OP_EFFECTS_SAVE, target) == 28 &&
                                    offsetof(OP_EFFECTS_SAVE, cached) == 32 &&
                                    offsetof(OP_EFFECTS_SAVE, remaining) == 36 &&
                                    offsetof(OP_EFFECTS_SAVE, restore) == 40 && offsetof(OP_EFFECTS_SAVE, saved) == 44
                                ? 1
                                : -1];
typedef char op_ripple_stride[sizeof(OP_RIPPLE) == 72 ? 1 : -1];
typedef char
    op_material_stride[sizeof(OP_RIPPLE_MATERIAL) == 12 && offsetof(OP_RIPPLE_MATERIAL, material) == 8 ? 1 : -1];
extern OP_EFFECTS_SAVE op_effects_save;
extern float op_fog_duration, op_fog_target, op_fog_cached_start, op_fog_remaining;
extern int op_fog_restore_mode;
extern unsigned int op_fog_saved;
extern OP_HALO op_halos[32];
extern OP_RIPPLE op_ripples[256];
extern OP_RIPPLE_MATERIAL op_ripple_materials[8];
extern void *op_zap_sprite;
extern float op_zap_points[];
extern int op_model_ambient_boost;
int op_save_read(void *memory, unsigned int bytes);
int op_shield_load(void);
int op_overlay_read_state(void);
void op_halo_draw_slot(OP_ACTOR *actor, int slot);
void op_halo_attach(OP_ACTOR *actor);
void *op_acquire_sprite(char *name);
void op_release_sprite(void **sprite);
void *op_sprite_get_material(void *sprite);
void *op_compose_materials(void *color, void *alpha);
void op_clear_projected_overlays(void);
void op_destroy_material(void *material);
void *op_resolve_zap_material(void *sprite);
void op_generate_zap_points(float *start, float *end, int depth);
void op_submit_zap_points(float *points, int count, unsigned int color, void *material, float width);
int op_effects_load(int kind);
void op_halo_draw_actor(OP_ACTOR *actor);
void *op_ripple_material(int appearance);
void op_ripple_reset(void);
void op_zap_draw_segment(float *start, float *end, int depth, float width, unsigned int color);
void op_zap_draw_default(float *start, float *end);
void op_effects_object_visibility(OP_ACTOR *actor);
void op_effects_object_created(OP_ACTOR *actor);
#endif
