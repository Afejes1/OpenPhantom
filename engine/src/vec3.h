#ifndef OP_VEC3_H
#define OP_VEC3_H
#include <stddef.h>
typedef struct OP_VEC3
{
    float x, y, z;
} OP_VEC3;
typedef char op_vec3_layout[(sizeof(OP_VEC3) == 12 && offsetof(OP_VEC3, y) == 4 && offsetof(OP_VEC3, z) == 8) ? 1 : -1];
#endif
