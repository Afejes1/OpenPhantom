#ifndef OP_CAMPAIGN043_API_H
#define OP_CAMPAIGN043_API_H
#include <stddef.h>
typedef struct OP_RASTER
{
    unsigned char prefix[12];
    int width, height;
} OP_RASTER;
typedef struct OP_CANVAS
{
    unsigned int flags;
    OP_RASTER *raster;
    float center_x, center_y;
    unsigned int field10, field14;
    int left, top, right, bottom;
} OP_CANVAS;
typedef struct OP_RD_FACE
{
    unsigned int field00, field04, unknown08[3], field14;
    void *array18, *array1c;
    unsigned int field20;
    int field24;
    unsigned int vector28[2], field30, unknown34, vector38[3], field44, field48;
} OP_RD_FACE;
typedef char canvas_layout
    [(sizeof(OP_CANVAS) == 40 && offsetof(OP_CANVAS, left) == 24 && offsetof(OP_CANVAS, field14) == 20) ? 1 : -1];
typedef char raster_view[(sizeof(OP_RASTER) == 20 && offsetof(OP_RASTER, width) == 12) ? 1 : -1];
typedef char face_layout[(sizeof(OP_RD_FACE) == 76) ? 1 : -1];
extern const float op_canvas_two;
void *op_allocate(unsigned int);
void op_release(void *);
OP_CANVAS *op_canvas_new(unsigned int, OP_RASTER *, unsigned int, int, int, int, int, unsigned int);
int op_canvas_init(OP_CANVAS *, unsigned int, OP_RASTER *, unsigned int, int, int, int, int, unsigned int);
void op_canvas_free(OP_CANVAS *);
void op_canvas_free_inner(OP_CANVAS *);
OP_RD_FACE *op_face_new(void);
int op_face_init(OP_RD_FACE *);
#endif
