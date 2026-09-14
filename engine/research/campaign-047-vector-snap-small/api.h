#ifndef OP_CAMPAIGN047_API_H
#define OP_CAMPAIGN047_API_H
#include <stddef.h>
typedef struct OP_VECTOR3
{
    float x, y, z;
} OP_VECTOR3;
typedef char
    vector_layout[(sizeof(OP_VECTOR3) == 12 && offsetof(OP_VECTOR3, y) == 4 && offsetof(OP_VECTOR3, z) == 8) ? 1 : -1];
#ifdef __cplusplus
extern "C"
{
#endif
    extern const float op_geometry_zero, op_geometry_right_angle;
    float op_scalar_angle(float);
    float op_point_plane_distance(const OP_VECTOR3 *, const OP_VECTOR3 *, const OP_VECTOR3 *);
    float op_vector_angle(const OP_VECTOR3 *, const OP_VECTOR3 *);
    void op_vector_snap_small(OP_VECTOR3 *, float);
#ifdef __cplusplus
}
#endif
#endif
