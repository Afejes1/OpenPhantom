#ifndef OP_FOCUSED_ACCESSORS_H
#define OP_FOCUSED_ACCESSORS_H
#include <stddef.h>
typedef struct OP_SHIELD
{
    void *owner;
    int visible;
    unsigned char unknown08[4];
    int visibility_bypass;
    unsigned char before_colour[0x28];
    unsigned char red, green, blue, alpha;
    unsigned char before_radius[0x10];
    float radius;
    unsigned char tail[0x64];
} OP_SHIELD;
extern OP_SHIELD op_shields[32];
typedef char shield_stride[sizeof(OP_SHIELD) == 0xb4 ? 1 : -1];
typedef char visible_offset[offsetof(OP_SHIELD, visible) == 4 ? 1 : -1];
typedef char bypass_offset[offsetof(OP_SHIELD, visibility_bypass) == 0xc ? 1 : -1];
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
#endif
