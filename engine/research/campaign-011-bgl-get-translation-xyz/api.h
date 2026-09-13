#ifndef OP_C011_BGL_GET_TRANSLATION_XYZ_API_H
#define OP_C011_BGL_GET_TRANSLATION_XYZ_API_H
typedef struct OP_BGL_MATRIX { unsigned int words[12]; } OP_BGL_MATRIX;
extern OP_BGL_MATRIX *op_bgl_current;
extern int op_bgl_depth;
void op_bgl_get_translation_xyz(unsigned int *x, unsigned int *y, unsigned int *z);
#endif
