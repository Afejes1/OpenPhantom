#ifndef OP_CAMPAIGN_013_BGL_PERP_AXIS_API_H
#define OP_CAMPAIGN_013_BGL_PERP_AXIS_API_H

typedef struct OP_VEC3 { float v[3]; } OP_VEC3;
typedef struct OP_MATRIX { unsigned long words[12]; } OP_MATRIX;

void op_build_rotation(OP_MATRIX *output, const OP_VEC3 *axis, float angle);
void op_rotate_basis(OP_VEC3 *output, const OP_VEC3 *input,
                     const OP_MATRIX *matrix);
void op_bgl_perp_axis(OP_VEC3 *output, const OP_VEC3 *input);

#endif
