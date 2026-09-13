#ifndef OP_C011_BGL_PUSH_API_H
#define OP_C011_BGL_PUSH_API_H
typedef struct OP_BGL_MATRIX { unsigned int words[12]; } OP_BGL_MATRIX;
extern OP_BGL_MATRIX *op_bgl_current;
extern int op_bgl_depth;
extern OP_BGL_MATRIX op_bgl_stack[64];
void op_bgl_push(void);
#endif
