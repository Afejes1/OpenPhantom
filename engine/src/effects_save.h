#ifndef OP_EFFECTS_SAVE_H
#define OP_EFFECTS_SAVE_H
#include "effects_state.h"
typedef struct OP_EFFECTS_SAVE
{
    unsigned char prefix[24];
    float duration, target, cached, remaining;
    int restore;
    unsigned int saved;
    unsigned char tail[48];
} OP_EFFECTS_SAVE;
typedef char op_effects_save_size[sizeof(OP_EFFECTS_SAVE) == 96 ? 1 : -1];
typedef char
    op_effects_save_offsets[offsetof(OP_EFFECTS_SAVE, duration) == 24 && offsetof(OP_EFFECTS_SAVE, target) == 28 &&
                                    offsetof(OP_EFFECTS_SAVE, cached) == 32 &&
                                    offsetof(OP_EFFECTS_SAVE, remaining) == 36 &&
                                    offsetof(OP_EFFECTS_SAVE, restore) == 40 && offsetof(OP_EFFECTS_SAVE, saved) == 44
                                ? 1
                                : -1];
extern OP_EFFECTS_SAVE op_effects_save;
int op_save_read(void *memory, unsigned int bytes);
int op_shield_load(void);
int op_overlay_read_state(void);
int op_effects_load(int kind);
#endif
