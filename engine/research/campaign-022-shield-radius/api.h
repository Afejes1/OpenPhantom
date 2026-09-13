#ifndef OP_SHIELD_RADIUS_API_H
#define OP_SHIELD_RADIUS_API_H
#include <stddef.h>
typedef struct OP_SHIELD
{
    void *owner;
    unsigned char before_radius[0x48];
    float radius;
    unsigned char tail[0x64];
} OP_SHIELD;
extern OP_SHIELD op_shields[32];
typedef char owner_offset[offsetof(OP_SHIELD, owner) == 0 ? 1 : -1];
typedef char radius_offset[offsetof(OP_SHIELD, radius) == 0x4c ? 1 : -1];
typedef char shield_size[sizeof(OP_SHIELD) == 0xb4 ? 1 : -1];
typedef char scalar_width[sizeof(int) == 4 && sizeof(void *) == 4 && sizeof(float) == 4 ? 1 : -1];
float op_shield_radius(int slot);
#endif
