#ifndef OP_EFFECTS_CONTROL_API_H
#define OP_EFFECTS_CONTROL_API_H
#include <stddef.h>
typedef struct OP_ATTACHED_ACTOR
{
    unsigned char before_slot[0x100];
    int shield_slot;
} OP_ATTACHED_ACTOR;
typedef struct OP_SHIELD
{
    int active;
    int visible;
    int stopping;
    int no_save;
    OP_ATTACHED_ACTOR *attached;
    float elapsed;
    unsigned char before_offsets[0x38];
    float offset_x, offset_y, offset_z;
    void *sprite;
    char name[0x54];
} OP_SHIELD;
typedef char op_shield_size[sizeof(OP_SHIELD) == 0xb4 ? 1 : -1];
typedef char op_shield_stopping[offsetof(OP_SHIELD, stopping) == 8 ? 1 : -1];
typedef char op_shield_no_save[offsetof(OP_SHIELD, no_save) == 0xc ? 1 : -1];
typedef char op_shield_attached[offsetof(OP_SHIELD, attached) == 0x10 ? 1 : -1];
typedef char op_shield_elapsed[offsetof(OP_SHIELD, elapsed) == 0x14 ? 1 : -1];
typedef char op_shield_sprite[offsetof(OP_SHIELD, sprite) == 0x5c ? 1 : -1];
typedef char op_shield_name[offsetof(OP_SHIELD, name) == 0x60 ? 1 : -1];
typedef char op_actor_slot[offsetof(OP_ATTACHED_ACTOR, shield_slot) == 0x100 ? 1 : -1];
typedef char op_word_sizes[sizeof(void *) == 4 && sizeof(int) == 4 && sizeof(float) == 4 ? 1 : -1];
extern OP_SHIELD op_shields[32];
void op_release(void *memory);
void *op_allocate(unsigned int bytes);
int op_shield_destroy(int slot);
void op_release_sprite(void **sprite);
void *op_acquire_sprite(char *name);
void op_shield_free(void *memory);
void op_shield_destroy_all(void);
int op_shield_stop(int slot);
int op_shield_save_size(void);
void *op_shield_calloc(unsigned int bytes);
int op_shield_set_texture(int slot, char *name);
typedef char op_shield_offset_x[offsetof(OP_SHIELD, offset_x) == 0x50 ? 1 : -1];
typedef char op_shield_offset_y[offsetof(OP_SHIELD, offset_y) == 0x54 ? 1 : -1];
typedef char op_shield_offset_z[offsetof(OP_SHIELD, offset_z) == 0x58 ? 1 : -1];
typedef struct OP_WORLD
{
    unsigned char before_flags[0x210];
    unsigned int flags;
} OP_WORLD;
typedef char op_world_flags_offset[offsetof(OP_WORLD, flags) == 0x210 ? 1 : -1];
extern OP_WORLD *op_active_world;
extern float op_fog_remaining, op_fog_cached_start;
extern unsigned int op_fog_saved;
extern int op_tint_active, op_tint_mode;
extern float op_tint_start, op_tint_duration;
extern int op_shield_count, op_shield_pass, op_shield_other_pass;
void op_world_apply_fog(OP_WORLD *world);
void op_renderer_fog_start(float value);
void op_renderer_fog_end(float value);
void op_tint_start_full(int mode, float duration, int hold, unsigned char r, unsigned char g, unsigned char b,
                        unsigned char a);
void op_shield_draw(int slot);
void op_fog_snapshot(void);
void op_fog_restore(void);
void op_fog_set_start(float value);
void op_fog_set_end(float value);
void op_tint_start_opaque(int mode, float duration, int hold, unsigned char r, unsigned char g, unsigned char b);
void op_tint_stop(void);
OP_SHIELD *op_shield_set_offset(int slot, float x, float y, float z);
void op_shield_draw_released(void);
#endif
