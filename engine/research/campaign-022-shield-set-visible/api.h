#ifndef OP_SHIELD_SET_API_H
#define OP_SHIELD_SET_API_H
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
int op_shield_set_visible(int slot, int value);
#endif
