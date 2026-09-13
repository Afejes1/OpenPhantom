#ifndef OP_MODEL_LOAD_API_H
#define OP_MODEL_LOAD_API_H
#include <stddef.h>
typedef struct OP_MODEL
{
    unsigned char before_state[0xd0];
    int load_state;
} OP_MODEL;
typedef char model_state_offset[offsetof(OP_MODEL, load_state) == 0xd0 ? 1 : -1];
typedef char model_extent[sizeof(OP_MODEL) == 0xd4 ? 1 : -1];
typedef char int_width[sizeof(int) == 4 ? 1 : -1];
int op_model_load_state(const OP_MODEL *model);
#endif
