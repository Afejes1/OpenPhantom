#ifndef OP_CAMPAIGN_007_SOUND_ACTIVATE_PLACE_API_H
#define OP_CAMPAIGN_007_SOUND_ACTIVATE_PLACE_API_H
typedef struct op_sound_placement { unsigned char opaque_00[0x20]; float random_span; unsigned char opaque_24[4]; float deadline; unsigned char opaque_2c[4]; int active; unsigned char opaque_34[8]; } op_sound_placement;
typedef struct op_sound_world_state { unsigned char opaque_00[0x54]; float time; } op_sound_world_state;
extern int op_sound_initialized;
extern op_sound_world_state *op_sound_world;
int op_sound_random(void);
void op_sound_activate_place(op_sound_placement *placement);
#endif