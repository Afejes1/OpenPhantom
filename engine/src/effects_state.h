#ifndef OP_EFFECTS_STATE_H
#define OP_EFFECTS_STATE_H
#include "b3d_chunk_readers.h"

typedef union OP_COLOR_WORD {
    unsigned int word;
    unsigned char value;
} OP_COLOR_WORD;
typedef char op_color_word_size[sizeof(OP_COLOR_WORD) == 4 ? 1 : -1];
extern OP_B3D_WORLD *op_active_world;
extern float op_fog_remaining, op_fog_target, op_fog_cached_start, op_fog_duration;
extern int op_fog_restore_mode;
extern unsigned int op_fog_saved;
extern int op_tint_active, op_tint_completed, op_tint_mode, op_tint_hold;
extern float op_tint_start, op_tint_duration;
extern OP_COLOR_WORD op_tint_red, op_tint_green, op_tint_blue, op_tint_alpha;
extern int op_letterbox_target, op_letterbox_previous, op_letterbox_step;
/* Observed ST(0) return and caller binary32 store; producer source precision unresolved. */
double op_clock_seconds(void);
void op_renderer_fog_start(float value);
void op_renderer_fog_end(float value);
void op_fog_snapshot(void);
void op_fog_restore(void);
void op_fog_set_start(float value);
void op_fog_set_end(float value);
void op_start_timed_fog_transition(int duration, float target, int red, int green, int blue, int restore);
void op_fog_tick(float delta);
void op_tint_start_full(int mode, float duration, int hold, unsigned char red, unsigned char green, unsigned char blue,
                        unsigned char alpha);
void op_tint_start_opaque(int mode, float duration, int hold, unsigned char red, unsigned char green,
                          unsigned char blue);
void op_tint_stop(void);
void op_fade_reset(void);
#endif
