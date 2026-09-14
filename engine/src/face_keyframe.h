#ifndef OP_FACE_KEYFRAME_H
#define OP_FACE_KEYFRAME_H
#include <stddef.h>
typedef struct OP_RD_FACE
{
    unsigned int field00, field04, unknown08[3], field14;
    void *array18, *array1c;
    unsigned int field20;
    int field24;
    unsigned int vector28[2], field30, unknown34, vector38[3], field44, field48;
} OP_RD_FACE;
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
typedef OP_KEYFRAME *(*OP_KEYFRAME_LOAD_HOOK)(char *);
typedef void (*OP_KEYFRAME_UNLOAD_HOOK)(OP_KEYFRAME *);
typedef char face_extent[(sizeof(OP_RD_FACE) == 76) ? 1 : -1];
typedef char face_array18[(offsetof(OP_RD_FACE, array18) == 24) ? 1 : -1];
typedef char face_vector38[(offsetof(OP_RD_FACE, vector38) == 56) ? 1 : -1];
typedef char keyframe_extent[(sizeof(OP_KEYFRAME) == 132) ? 1 : -1];
extern OP_KEYFRAME_LOAD_HOOK op_keyframe_load_hook;
extern OP_KEYFRAME_UNLOAD_HOOK op_keyframe_unload_hook;
extern char op_default_resource_name[];
void op_release(void *);
char *op_copy_keyframe_name(char *, const char *, unsigned int);
int op_face_init(OP_RD_FACE *);
void op_face_free(OP_RD_FACE *);
void op_face_free_arrays(OP_RD_FACE *);
OP_KEYFRAME_LOAD_HOOK op_keyframe_set_load_hook(OP_KEYFRAME_LOAD_HOOK);
OP_KEYFRAME_UNLOAD_HOOK op_keyframe_set_unload_hook(OP_KEYFRAME_UNLOAD_HOOK);
void op_keyframe_init_header(OP_KEYFRAME *);
typedef char
    keyframe_node_extent[(sizeof(OP_KEYFRAME_NODE) == 44 && offsetof(OP_KEYFRAME_NODE, payload) == 40) ? 1 : -1];
typedef char
    keyframe_nodes_offset[(offsetof(OP_KEYFRAME, node_count) == 56 && offsetof(OP_KEYFRAME, nodes) == 60) ? 1 : -1];
void *op_allocate(unsigned int);
int op_keyframe_load_entry(char *, OP_KEYFRAME *);
OP_KEYFRAME *op_keyframe_load(char *);
void op_keyframe_free(OP_KEYFRAME *);
void op_keyframe_free_entry(OP_KEYFRAME *);
#endif
