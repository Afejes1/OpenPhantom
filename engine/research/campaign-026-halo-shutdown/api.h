#ifndef OP_CAMPAIGN026_API_H
#define OP_CAMPAIGN026_API_H
#include <stddef.h>
typedef struct OP_OVERLAY_SAVE
{
    unsigned char prefix[12];
    int visible, target, previous, step;
} OP_OVERLAY_SAVE;
typedef struct OP_ACTOR
{
    unsigned int flags;
    unsigned char before_slot[0xfc];
    int shield_slot;
} OP_ACTOR;
typedef struct OP_HALO
{
    OP_ACTOR *owner;
    unsigned char before_sprite[16];
    void *sprite;
    unsigned char tail[4];
} OP_HALO;
typedef char op_save_size[sizeof(OP_OVERLAY_SAVE) == 28 ? 1 : -1];
typedef char op_save_visible[offsetof(OP_OVERLAY_SAVE, visible) == 12 ? 1 : -1];
typedef char op_save_target[offsetof(OP_OVERLAY_SAVE, target) == 16 ? 1 : -1];
typedef char op_save_previous[offsetof(OP_OVERLAY_SAVE, previous) == 20 ? 1 : -1];
typedef char op_save_step[offsetof(OP_OVERLAY_SAVE, step) == 24 ? 1 : -1];
typedef char op_actor_slot[offsetof(OP_ACTOR, shield_slot) == 0x100 ? 1 : -1];
typedef char op_halo_size[sizeof(OP_HALO) == 28 ? 1 : -1];
typedef char op_halo_sprite[offsetof(OP_HALO, sprite) == 20 ? 1 : -1];
extern int op_letterbox_visible, op_letterbox_target, op_letterbox_previous, op_letterbox_step;
extern OP_OVERLAY_SAVE op_overlay_save;
extern char op_halo_name_a[], op_halo_name_b[], op_halo_name_c[];
extern void *op_halo_sprite_a, *op_halo_sprite_b, *op_halo_sprite_c;
extern OP_HALO op_halos[32];
extern int op_halo_count;
void op_save_write(const void *memory, unsigned int bytes);
int op_save_read(void *memory, unsigned int bytes);
void *op_acquire_sprite(char *name);
void op_release_sprite(void **sprite);
void op_halo_draw_actor(OP_ACTOR *actor);
void op_shield_draw_attached(int slot, float *transform);
void op_letterbox_set_enabled(int enabled);
int op_overlay_save_state(void);
int op_overlay_read_state(void);
int op_overlay_restore_state(void);
void op_halo_startup(void);
void op_halo_shutdown(void);
void op_effects_draw_object(OP_ACTOR *actor, float *transform, int unused, int draw_halo);
void op_detach_halo(OP_ACTOR *actor);
#endif
