#ifndef OP_HALO_OVERLAY_H
#define OP_HALO_OVERLAY_H
#include "save_stream.h"
#include "focused_accessors.h"
typedef struct OP_OVERLAY_SAVE
{
    unsigned char prefix[12];
    int visible, target, previous, step;
} OP_OVERLAY_SAVE;
typedef struct OP_HALO
{
    OP_ATTACHED_ACTOR *owner;
    int node, field8, fieldc, mode;
    void *sprite;
    unsigned int color;
} OP_HALO;
typedef char op_save_size[sizeof(OP_OVERLAY_SAVE) == 28 ? 1 : -1];
typedef char op_save_visible[offsetof(OP_OVERLAY_SAVE, visible) == 12 ? 1 : -1];
typedef char op_save_target[offsetof(OP_OVERLAY_SAVE, target) == 16 ? 1 : -1];
typedef char op_save_previous[offsetof(OP_OVERLAY_SAVE, previous) == 20 ? 1 : -1];
typedef char op_save_step[offsetof(OP_OVERLAY_SAVE, step) == 24 ? 1 : -1];
typedef char op_actor_slot[offsetof(OP_ATTACHED_ACTOR, shield_slot) == 0x100 ? 1 : -1];
typedef char op_halo_size[sizeof(OP_HALO) == 28 ? 1 : -1];
typedef char op_halo_sprite[offsetof(OP_HALO, sprite) == 20 ? 1 : -1];
extern int op_letterbox_visible, op_letterbox_target, op_letterbox_previous, op_letterbox_step;
extern OP_OVERLAY_SAVE op_overlay_save;
extern char op_halo_name_a[], op_halo_name_b[], op_halo_name_c[];
extern void *op_halo_sprite_a, *op_halo_sprite_b, *op_halo_sprite_c;
extern OP_HALO op_halos[32];
extern int op_halo_count;
void *op_acquire_sprite(char *name);
int op_release_sprite(void **sprite);
void op_halo_draw_actor(OP_ATTACHED_ACTOR *actor);
void op_shield_draw_attached(int slot, float *transform);
void op_letterbox_set_enabled(int enabled);
int op_overlay_save_state(void);
int op_overlay_read_state(void);
void op_halo_startup(void);
void op_halo_shutdown(void);
void op_effects_draw_object(OP_ATTACHED_ACTOR *actor, float *transform, int unused, int draw_halo);
void op_detach_halo(OP_ATTACHED_ACTOR *actor);
extern int op_model_ambient_boost;
void op_halo_free_all(void);
void op_halo_draw_slot(OP_ATTACHED_ACTOR *actor, int slot);
void op_halo_attach(OP_ATTACHED_ACTOR *actor);
void op_effects_object_visibility(OP_ATTACHED_ACTOR *actor);
void op_effects_object_created(OP_ATTACHED_ACTOR *actor);
typedef struct OP_HALO_COLOR
{
    char *name;
    unsigned int color;
} OP_HALO_COLOR;
extern OP_HALO_COLOR op_halo_colors[];
int op_find_node_ordinal(OP_ATTACHED_ACTOR *, unsigned int);
int op_compare_names(char *, char *);
void op_halo_add(OP_ATTACHED_ACTOR *, unsigned int, int, int, char *, unsigned int, int);
typedef char
    halo_offsets[(offsetof(OP_HALO, node) == 4 && offsetof(OP_HALO, field8) == 8 && offsetof(OP_HALO, fieldc) == 12 &&
                  offsetof(OP_HALO, mode) == 16 && offsetof(OP_HALO, sprite) == 20 && offsetof(OP_HALO, color) == 24 &&
                  sizeof(OP_HALO_COLOR) == 8)
                     ? 1
                     : -1];
#endif
