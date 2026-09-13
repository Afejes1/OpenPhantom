#ifndef OP_FOCUSED_ACCESSORS_H
#define OP_FOCUSED_ACCESSORS_H
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
    int count_18;
    void *buffer_1c, *buffer_20, *buffer_24, *buffer_28;
    void *buffer_2c, *buffer_30, *buffer_34;
    unsigned char red, green, blue, alpha;
    int count_3c;
    void *buffer_40;
    unsigned char unknown44[8];
    float radius;
    float offset_x, offset_y, offset_z;
    void *sprite;
    /* Extent view to the next record, not a recovered string capacity. */
    char name[0x54];
} OP_SHIELD;
extern OP_SHIELD op_shields[32];
typedef char shield_stride[sizeof(OP_SHIELD) == 0xb4 ? 1 : -1];
typedef char visible_offset[offsetof(OP_SHIELD, visible) == 4 ? 1 : -1];
typedef char bypass_offset[offsetof(OP_SHIELD, no_save) == 0xc ? 1 : -1];
typedef char colour_offset[offsetof(OP_SHIELD, red) == 0x38 && offsetof(OP_SHIELD, green) == 0x39 &&
                                   offsetof(OP_SHIELD, blue) == 0x3a && offsetof(OP_SHIELD, alpha) == 0x3b
                               ? 1
                               : -1];
typedef char radius_offset[offsetof(OP_SHIELD, radius) == 0x4c ? 1 : -1];
typedef char word_width[sizeof(int) == 4 && sizeof(void *) == 4 && sizeof(float) == 4 ? 1 : -1];
typedef struct OP_MODEL
{
    unsigned char before_state[0xd0];
    int load_state;
} OP_MODEL;
typedef char model_load_offset[offsetof(OP_MODEL, load_state) == 0xd0 ? 1 : -1];
typedef char model_partial_size[sizeof(OP_MODEL) == 0xd4 ? 1 : -1];
typedef struct OP_FONT OP_FONT;
extern OP_FONT *op_system_font;
int op_model_load_state(const OP_MODEL *model);
OP_FONT *op_get_system_font(void);
float op_shield_radius(int slot);
int op_shield_set_colour(int slot, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
int op_shield_set_visible(int slot, int value);
/* Field0C suppresses saves and bypasses view-volume classification. */
int op_shield_set_visibility_bypass(int slot, int value);

typedef char shield_actor_slot[offsetof(OP_ATTACHED_ACTOR, shield_slot) == 0x100 ? 1 : -1];
typedef char shield_active_offset[offsetof(OP_SHIELD, active) == 0x0 ? 1 : -1];
typedef char shield_stopping_offset[offsetof(OP_SHIELD, stopping) == 0x8 ? 1 : -1];
typedef char shield_attached_offset[offsetof(OP_SHIELD, attached) == 0x10 ? 1 : -1];
typedef char shield_elapsed_offset[offsetof(OP_SHIELD, elapsed) == 0x14 ? 1 : -1];
typedef char shield_count_18_offset[offsetof(OP_SHIELD, count_18) == 0x18 ? 1 : -1];
typedef char shield_buffer_1c_offset[offsetof(OP_SHIELD, buffer_1c) == 0x1c ? 1 : -1];
typedef char shield_buffer_20_offset[offsetof(OP_SHIELD, buffer_20) == 0x20 ? 1 : -1];
typedef char shield_buffer_24_offset[offsetof(OP_SHIELD, buffer_24) == 0x24 ? 1 : -1];
typedef char shield_buffer_28_offset[offsetof(OP_SHIELD, buffer_28) == 0x28 ? 1 : -1];
typedef char shield_buffer_2c_offset[offsetof(OP_SHIELD, buffer_2c) == 0x2c ? 1 : -1];
typedef char shield_buffer_30_offset[offsetof(OP_SHIELD, buffer_30) == 0x30 ? 1 : -1];
typedef char shield_buffer_34_offset[offsetof(OP_SHIELD, buffer_34) == 0x34 ? 1 : -1];
typedef char shield_count_3c_offset[offsetof(OP_SHIELD, count_3c) == 0x3c ? 1 : -1];
typedef char shield_buffer_40_offset[offsetof(OP_SHIELD, buffer_40) == 0x40 ? 1 : -1];
typedef char shield_offset_x_offset[offsetof(OP_SHIELD, offset_x) == 0x50 ? 1 : -1];
typedef char shield_offset_y_offset[offsetof(OP_SHIELD, offset_y) == 0x54 ? 1 : -1];
typedef char shield_offset_z_offset[offsetof(OP_SHIELD, offset_z) == 0x58 ? 1 : -1];
typedef char shield_sprite_offset[offsetof(OP_SHIELD, sprite) == 0x5c ? 1 : -1];
typedef char shield_name_offset[offsetof(OP_SHIELD, name) == 0x60 ? 1 : -1];

extern int op_shield_count, op_shield_pass, op_shield_other_pass;
void *op_allocate(unsigned int bytes);
void op_release(void *memory);
void op_release_sprite(void **sprite);
void *op_acquire_sprite(char *name);
void op_shield_draw(int slot);
void op_detach_halo(OP_ATTACHED_ACTOR *object);
void op_detach_zap(OP_ATTACHED_ACTOR *object);
void *op_shield_calloc(unsigned int bytes);
void op_shield_free(void *memory);
int op_shield_destroy(int slot);
void op_shield_destroy_all(void);
int op_shield_stop(int slot);
int op_shield_save_size(void);
int op_shield_set_texture(int slot, char *name);
void op_shield_draw_released(void);
void op_effects_object_destroyed(OP_ATTACHED_ACTOR *object);
#endif
