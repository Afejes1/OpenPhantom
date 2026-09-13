#ifndef OP_CAMPAIGN_015_EXTENDED_ADVANCE_CEL_API_H
#define OP_CAMPAIGN_015_EXTENDED_ADVANCE_CEL_API_H

typedef struct OP_EXTENDED_MATERIAL {
    unsigned char opaque_00[0x64];
    int cel_count;
} OP_EXTENDED_MATERIAL;

typedef struct OP_EXTENDED_FACE {
    unsigned char opaque_00[0x20];
    OP_EXTENDED_MATERIAL *material;
    int cel;
} OP_EXTENDED_FACE;

void op_extended_advance_cel(void *ignored_context, OP_EXTENDED_FACE *face);

#endif
