#ifndef OP_CAMPAIGN029_API_H
#define OP_CAMPAIGN029_API_H
#include <stddef.h>
typedef struct OP_DEFINITION
{
    unsigned char prefix[8];
    char name[32];
} OP_DEFINITION;
typedef struct OP_ACTOR
{
    unsigned int flags;
    unsigned char before_definition[16];
    OP_DEFINITION *definition;
    unsigned char before_slot[232];
    int slot;
} OP_ACTOR;
typedef struct OP_WORLD
{
    unsigned char prefix[528];
    unsigned int flags;
} OP_WORLD;
typedef struct OP_EFFECTS_SAVE
{
    int shield_size, overlay_size;
    unsigned char unused[16];
    float duration, target, cached, remaining;
    int restore;
    unsigned int saved, enabled;
    int red, green, blue;
    float start, end;
    unsigned char tail[24];
} OP_EFFECTS_SAVE;
typedef struct OP_HALO
{
    OP_ACTOR *owner;
    int node, field8, fieldc, mode;
    void *sprite;
    unsigned int color;
} OP_HALO;
typedef struct OP_HALO_COLOR
{
    char *name;
    unsigned int color;
} OP_HALO_COLOR;
typedef struct OP_MATERIAL
{
    unsigned char prefix[176];
    unsigned char *frame;
} OP_MATERIAL;
typedef struct OP_SPRITE
{
    unsigned char prefix[132];
    OP_MATERIAL *material;
} OP_SPRITE;
typedef struct OP_SHIELD
{
    int active, visible, stopping, no_save;
    OP_ACTOR *actor;
    float elapsed;
    unsigned char before_color[32];
    unsigned char red, green, blue, alpha;
    unsigned char before_radius[16];
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
typedef char op_layout[(sizeof(void *) == 4 && sizeof(OP_ACTOR) == 260 && offsetof(OP_ACTOR, definition) == 20 &&
                        offsetof(OP_ACTOR, slot) == 256 && sizeof(OP_EFFECTS_SAVE) == 96 && sizeof(OP_HALO) == 28 &&
                        sizeof(OP_SHIELD) == 180 && offsetof(OP_SHIELD, name) == 96 &&
                        offsetof(OP_SHIELD, saved_actor) == 176 && sizeof(OP_SHIELD_SAVE) == 52)
                           ? 1
                           : -1];
extern OP_EFFECTS_SAVE op_effects_save;
extern float op_fog_duration, op_fog_target, op_fog_cached_start, op_fog_remaining;
extern int op_fog_restore_mode;
extern unsigned int op_fog_saved;
extern OP_WORLD *op_active_world;
extern OP_HALO op_halos[32];
extern int op_halo_count;
extern OP_HALO_COLOR op_halo_colors[];
extern char op_halo_name_a[], op_halo_name_b[], op_halo_name_c[], op_sprite_format[];
extern OP_SHIELD op_shields[32];
extern void *op_object_pool;
void op_get_fog_rgb(int *, int *, int *);
void op_get_fog_range(float *, float *);
int op_shield_save_size(void);
int op_overlay_save_size(void);
void op_save_header(int, int, int);
void op_save_write(void *, unsigned int);
int op_shield_write(void);
int op_overlay_save_state(void);
unsigned int op_renderer_get_flags(void);
void op_renderer_set_flags(unsigned int);
void op_renderer_set_fog_start(float);
void op_renderer_set_fog_end(float);
void op_renderer_set_fog_rgb(int, int, int);
void op_renderer_set_clear_rgb(int, int, int);
void op_shield_relink(void);
void op_overlay_restore_state(void);
int op_find_node_ordinal(OP_ACTOR *, unsigned int);
void *op_acquire_sprite(char *);
int op_compare_names(char *, char *);
int op_prepare_material_frame(OP_MATERIAL *, unsigned char *, int);
int op_format_sprite_name(char *, char *, ...);
void *op_acquire_resource(unsigned int, char *);
void op_release_resource(void *);
void *op_iterator_open(void *);
void op_iterator_reset(void *);
OP_ACTOR *op_iterator_next(void *);
void op_iterator_close(void *);
int op_effects_write(int);
void op_effects_restore(void);
void op_halo_add(OP_ACTOR *, unsigned int, int, int, char *, unsigned int, int);
void op_halo_attach(OP_ACTOR *);
void *op_sprite_frame(void *);
void *op_sprite_acquire(char *);
int op_sprite_release(void **);
#endif
