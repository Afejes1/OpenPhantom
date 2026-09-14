#ifndef OP_CAMPAIGN044_API_H
#define OP_CAMPAIGN044_API_H
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
typedef char matrix_layout
    [(sizeof(OP_MATRIX34) == 48 && offsetof(OP_MATRIX34, translation) == 36 && sizeof(OP_VECTOR3) == 12) ? 1 : -1];
void op_matrix_build_rotation(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_build_translation(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_build_scale(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_multiply_post(OP_MATRIX34 *, OP_MATRIX34 *);
void op_matrix_multiply_pre(OP_MATRIX34 *, OP_MATRIX34 *);
void op_matrix_post_rotate(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_pre_rotate(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_post_translate(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_pre_translate(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_post_scale(OP_MATRIX34 *, OP_VECTOR3 *);
void op_matrix_pre_scale(OP_MATRIX34 *, OP_VECTOR3 *);
#endif
