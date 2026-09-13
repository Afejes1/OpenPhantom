#ifndef OP_CAMPAIGN_006_SOUND_DEACTIVATE_PLACE_API_H
#define OP_CAMPAIGN_006_SOUND_DEACTIVATE_PLACE_API_H
typedef struct op_sound_placement { unsigned char opaque_00[0x30]; int active; unsigned char opaque_34[8]; } op_sound_placement;
extern int op_sound_initialized;
void op_sound_deactivate_place(op_sound_placement *placement);
#endif