#ifndef OP_C012_MULTIPLY_API_H
#define OP_C012_MULTIPLY_API_H
typedef struct OP_VEC3 { float x,y,z; } OP_VEC3;
typedef struct OP_MATRIX { float basis[9]; OP_VEC3 translation; } OP_MATRIX;
extern OP_MATRIX *op_bgl_current;
void op_multiply(OP_MATRIX *left,const OP_MATRIX *right);
void op_bgl_multiply(const OP_MATRIX *rhs);
#endif
