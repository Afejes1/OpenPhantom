#ifndef OP_CAMPAIGN_007_SOUND_RESET_PLACES_API_H
#define OP_CAMPAIGN_007_SOUND_RESET_PLACES_API_H
typedef struct op_sound_placement { unsigned char opaque_00[0x28]; float deadline; unsigned char opaque_2c[4]; int active; int initial_active; unsigned char opaque_38[4]; } op_sound_placement;
typedef struct op_sound_world_state { unsigned char opaque_00[0xcbc]; int placement_count; op_sound_placement *placements; } op_sound_world_state;
extern op_sound_world_state *op_sound_world;
void op_sound_reset_places(void);
#endif