#ifndef OP_C012_TRANSLATE_NEGATIVE_API_H
#define OP_C012_TRANSLATE_NEGATIVE_API_H
typedef struct OP_VEC3 { float x,y,z; } OP_VEC3;
typedef struct OP_MATRIX { float basis[9]; OP_VEC3 translation; } OP_MATRIX;
void op_bgl_translate_vector(const OP_VEC3 *input);
void op_bgl_translate_negative(const OP_VEC3 *input);
#endif
