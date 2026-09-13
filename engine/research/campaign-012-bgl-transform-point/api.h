#ifndef OP_C012_TRANSFORM_POINT_API_H
#define OP_C012_TRANSFORM_POINT_API_H
typedef struct OP_VEC3 { float x,y,z; } OP_VEC3;
typedef struct OP_MATRIX { float basis[9]; OP_VEC3 translation; } OP_MATRIX;
extern OP_MATRIX *op_bgl_current;
void op_rotate_basis(OP_VEC3 *out,const OP_VEC3 *input,const OP_MATRIX *matrix);
void op_bgl_transform_point(OP_VEC3 *point);
#endif
