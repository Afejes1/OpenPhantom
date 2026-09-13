#ifndef OP_C012_ROTATE_XYZ_API_H
#define OP_C012_ROTATE_XYZ_API_H
typedef struct OP_VEC3 { float x,y,z; } OP_VEC3;
typedef struct OP_MATRIX { float basis[9]; OP_VEC3 translation; } OP_MATRIX;
void op_bgl_rotate_axis(const OP_VEC3 *axis,float angle);
void op_bgl_rotate_xyz(float x,float y,float z,float angle);
#endif
