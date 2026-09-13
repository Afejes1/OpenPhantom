#ifndef OP_CAMPAIGN_010_SOUND_PLAY_BY_NAME_API_H
#define OP_CAMPAIGN_010_SOUND_PLAY_BY_NAME_API_H
typedef struct op_sound_descriptor {
    char name[24];
    unsigned int flags;
    float field0, field1, field2, field3, field4;
    int field5;
    unsigned char opaque34[8];
    int id;
} op_sound_descriptor;
extern op_sound_descriptor op_sound_default_descriptor;
extern float op_sound_field0, op_sound_field1, op_sound_field2;
extern float op_sound_field3, op_sound_field4;
extern int op_sound_field5, op_sound_category, op_sound_initialized;
int op_sound_start_descriptor(op_sound_descriptor *value, int *handle,
                              float *position);
int op_sound_play_by_name(int id, const char *name, int *handle,
                          float *position, unsigned int flags);
#endif
