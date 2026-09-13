#ifndef OP_CAMPAIGN_009_SOUND_LOAD_REF_API_H
#define OP_CAMPAIGN_009_SOUND_LOAD_REF_API_H
typedef struct op_sound_ref {
    unsigned char opaque_00[4];
    char name[52];
    void *resource;
} op_sound_ref;
extern int op_sound_load_failures;
void *op_sound_acquire_resource(unsigned int type, char *name);
int op_sound_load_ref(op_sound_ref *record);
#endif
