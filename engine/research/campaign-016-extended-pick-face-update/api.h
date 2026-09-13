#ifndef OP_CAMPAIGN_016_EXTENDED_PICK_FACE_UPDATE_API_H
#define OP_CAMPAIGN_016_EXTENDED_PICK_FACE_UPDATE_API_H

typedef struct OP_EXTENDED_FACE OP_EXTENDED_FACE;
typedef void (*OP_FACE_UPDATE)();

typedef struct OP_EXTENDED_MATERIAL {
    unsigned char opaque_00[0x64];
    unsigned int cel_count;
} OP_EXTENDED_MATERIAL;

struct OP_EXTENDED_FACE {
    unsigned char opaque_00[0x20];
    OP_EXTENDED_MATERIAL *material;
    unsigned int cel;
    unsigned char opaque_28[0x08];
    float trail_value;
    unsigned char opaque_34[0x10];
    unsigned int trail_state;
    OP_FACE_UPDATE update;
};

void op_capture_model_face_trail(void *thing, OP_EXTENDED_FACE *face,
                                 float *world_vertices);
void op_extended_advance_cel(void *ignored_context, OP_EXTENDED_FACE *face);
unsigned int op_random15(void);
void op_extended_pick_face_update(OP_EXTENDED_FACE *face);

#endif
