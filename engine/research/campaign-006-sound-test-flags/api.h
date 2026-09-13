#ifndef OP_CAMPAIGN_006_SOUND_TEST_FLAGS_API_H
#define OP_CAMPAIGN_006_SOUND_TEST_FLAGS_API_H
typedef struct op_sound_descriptor { unsigned char opaque_00[0x18]; unsigned int flags; unsigned char opaque_1c[0x24]; } op_sound_descriptor;
typedef struct op_sound_world_state { unsigned char opaque_00[0xcc8]; op_sound_descriptor *descriptors; } op_sound_world_state;
extern op_sound_world_state *op_sound_world;
unsigned int op_sound_test_flags(int index, unsigned int mask);
#endif