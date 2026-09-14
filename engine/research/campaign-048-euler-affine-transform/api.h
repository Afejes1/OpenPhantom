#ifndef OP_CAMPAIGN048_API_H
#define OP_CAMPAIGN048_API_H
#include <stddef.h>
typedef struct OP_VECTOR3
{
    float x, y, z;
} OP_VECTOR3;
typedef struct OP_MATRIX34
{
    float basis[9];
    OP_VECTOR3 translation;
} OP_MATRIX34;
typedef char layouts[(sizeof(OP_VECTOR3) == 12 && sizeof(OP_MATRIX34) == 48 && offsetof(OP_MATRIX34, translation) == 36)
                         ? 1
                         : -1];
float op_vector_normalize(OP_VECTOR3 *);
void op_vector_snap_small(OP_VECTOR3 *, float);
void op_angle_sincos(float, float *, float *);
void op_normal_from_three_points(OP_VECTOR3 *, const OP_VECTOR3 *, const OP_VECTOR3 *, const OP_VECTOR3 *);
void op_euler_affine_transform(OP_MATRIX34 *, const OP_VECTOR3 *, const OP_VECTOR3 *);
#endif
