#ifndef OP_CAMPAIGN_007_SOUND_CONTROL_PLACE_API_H
#define OP_CAMPAIGN_007_SOUND_CONTROL_PLACE_API_H
typedef struct op_sound_placement { unsigned char bytes[60]; } op_sound_placement;
typedef struct op_sound_world_state { unsigned char opaque_00[0xcbc]; int placement_count; op_sound_placement *placements; } op_sound_world_state;
extern int op_sound_initialized;
extern op_sound_world_state *op_sound_world;
void op_sound_activate_place(op_sound_placement *placement);
void op_sound_deactivate_place(op_sound_placement *placement);
void op_sound_control_place(int index, int enabled);
#endif