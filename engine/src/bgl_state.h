#ifndef OP_BGL_STATE_H
#define OP_BGL_STATE_H

typedef struct OP_BGL_MATRIX {
    unsigned int words[12];
} OP_BGL_MATRIX;

extern OP_BGL_MATRIX *op_bgl_current;
extern int op_bgl_depth;
extern OP_BGL_MATRIX op_bgl_stack[64];
extern const OP_BGL_MATRIX op_bgl_identity_matrix;

void op_bgl_identity(void);
void op_bgl_push(void);
void op_bgl_pop(void);
void op_bgl_load_matrix(const OP_BGL_MATRIX *source);
void op_bgl_get_matrix(OP_BGL_MATRIX *destination);
void op_bgl_get_translation(unsigned int *out3);
void op_bgl_get_translation_xyz(unsigned int *x, unsigned int *y,
                                unsigned int *z);

#endif