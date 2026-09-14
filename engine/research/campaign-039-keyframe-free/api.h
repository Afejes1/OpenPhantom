#ifndef OP_CAMPAIGN039_API_H
#define OP_CAMPAIGN039_API_H
#include <stddef.h>
typedef struct OP_KEYFRAME_NODE
{
    unsigned char prefix[40];
    void *payload;
} OP_KEYFRAME_NODE;
typedef struct OP_KEYFRAME
{
    char name[32];
    unsigned char unknown20[24];
    unsigned int node_count;
    OP_KEYFRAME_NODE *nodes;
    unsigned char unknown40[68];
} OP_KEYFRAME;
typedef struct OP_MATERIAL OP_MATERIAL;
typedef OP_KEYFRAME *(*OP_KEYFRAME_LOAD_HOOK)(char *);
typedef void (*OP_KEYFRAME_UNLOAD_HOOK)(OP_KEYFRAME *);
typedef OP_MATERIAL *(*OP_MATERIAL_LOAD_HOOK)(char *, void *, void *);
typedef void (*OP_MATERIAL_UNLOAD_HOOK)(OP_MATERIAL *);
typedef char node_extent[(sizeof(OP_KEYFRAME_NODE) == 44 && offsetof(OP_KEYFRAME_NODE, payload) == 40) ? 1 : -1];
typedef char frame_extent[(sizeof(OP_KEYFRAME) == 132 && offsetof(OP_KEYFRAME, node_count) == 56 &&
                           offsetof(OP_KEYFRAME, nodes) == 60)
                              ? 1
                              : -1];
extern OP_KEYFRAME_LOAD_HOOK op_keyframe_load_hook;
extern OP_KEYFRAME_UNLOAD_HOOK op_keyframe_unload_hook;
extern OP_MATERIAL_LOAD_HOOK op_material_load_hook;
extern OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
void *op_allocate(unsigned int);
void op_release(void *);
int op_keyframe_load_entry(char *, OP_KEYFRAME *);
OP_KEYFRAME *op_keyframe_load(char *);
void op_keyframe_free(OP_KEYFRAME *);
void op_keyframe_free_entry(OP_KEYFRAME *);
OP_MATERIAL_LOAD_HOOK op_material_set_load_hook(OP_MATERIAL_LOAD_HOOK);
OP_MATERIAL_UNLOAD_HOOK op_material_set_unload_hook(OP_MATERIAL_UNLOAD_HOOK);
#endif
