#ifndef OP_GEOMETRY_H
#define OP_GEOMETRY_H
#include <stddef.h>
#include "matrix34.h"

extern const float op_geometry_zero, op_geometry_right_angle;
float op_scalar_angle(float);
float op_point_plane_distance(const OP_VECTOR3 *, const OP_VECTOR3 *, const OP_VECTOR3 *);
float op_vector_angle(const OP_VECTOR3 *, const OP_VECTOR3 *);
#endif
