#ifndef OP_EFFECTS_SAVE_H
#define OP_EFFECTS_SAVE_H
#include "save_stream.h"
#include "effects_state.h"
typedef struct OP_EFFECTS_SAVE
{
    int shield_size, overlay_size;
    unsigned char unused[16];
    float duration, target, cached, remaining;
    int restore;
    unsigned int saved, enabled;
    int red, green, blue;
    float start, end;
    unsigned char tail[24];
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
int op_shield_load(void);
int op_overlay_read_state(void);
int op_effects_load(int kind);
typedef struct OP_SHIELD_SAVE
{
    int slot, visible;
    float elapsed;
    char name[32];
    float radius;
    unsigned int color;
} OP_SHIELD_SAVE;
typedef char shield_save_extent[sizeof(OP_SHIELD_SAVE) == 52 ? 1 : -1];
void op_get_fog_rgb(int *, int *, int *);
void op_get_fog_range(float *, float *);
int op_shield_save_size(void);
int op_overlay_save_size(void);
int op_overlay_save_state(void);
int op_shield_write(void);
int op_shield_allocate(void *);
int op_effects_write(int);
typedef char
    effects_extended_offsets[(offsetof(OP_EFFECTS_SAVE, shield_size) == 0 &&
                              offsetof(OP_EFFECTS_SAVE, overlay_size) == 4 &&
                              offsetof(OP_EFFECTS_SAVE, enabled) == 48 && offsetof(OP_EFFECTS_SAVE, red) == 52 &&
                              offsetof(OP_EFFECTS_SAVE, green) == 56 && offsetof(OP_EFFECTS_SAVE, blue) == 60 &&
                              offsetof(OP_EFFECTS_SAVE, start) == 64 && offsetof(OP_EFFECTS_SAVE, end) == 68 &&
                              offsetof(OP_SHIELD_SAVE, name) == 12 && offsetof(OP_SHIELD_SAVE, radius) == 44 &&
                              offsetof(OP_SHIELD_SAVE, color) == 48)
                                 ? 1
                                 : -1];
#endif
