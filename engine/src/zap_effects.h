#ifndef OP_ZAP_EFFECTS_H
#define OP_ZAP_EFFECTS_H
#include "focused_accessors.h"
typedef struct OP_ZAP
{
    int active;
    unsigned char before_mode[12];
    int mode;
    OP_ATTACHED_ACTOR *endpoints[4];
    unsigned char tail[84];
} OP_ZAP;
typedef char op_zap_stride[sizeof(OP_ZAP) == 120 ? 1 : -1];
typedef char op_zap_mode_offset[offsetof(OP_ZAP, mode) == 16 ? 1 : -1];
typedef char op_zap_endpoints_offset[offsetof(OP_ZAP, endpoints) == 20 ? 1 : -1];
extern OP_ZAP op_zaps[64];
extern int op_zap_count;
extern char op_zap_name[];
extern void *op_zap_sprite;
/* Only the start address and the caller's maximum17 points are established here. */
extern float op_zap_points[];
void op_detach_zap(OP_ATTACHED_ACTOR *actor);
int op_zap_startup(void);
int op_zap_shutdown(void);
void op_zap_reset(void);
int op_zap_has_actor(OP_ATTACHED_ACTOR *actor);
void *op_resolve_zap_material(void *sprite);
void op_generate_zap_points(float *start, float *end, int depth);
void op_submit_zap_points(float *points, int count, unsigned int color, void *material, float width);
void op_zap_draw_segment(float *start, float *end, int depth, float width, unsigned int color);
void op_zap_draw_default(float *start, float *end);
#endif
