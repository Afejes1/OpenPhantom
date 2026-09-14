#include "api.h"
// FUNCTION: WMAIN 0x0047691e
int op_canvas_init(OP_CANVAS *canvas, unsigned int flags, OP_RASTER *raster, unsigned int field10, int left, int top,
                   int right, int bottom, unsigned int field14)
{
    canvas->flags = flags;
    canvas->raster = raster;
    canvas->field10 = field10;
    canvas->field14 = field14;
    if (flags & 1)
    {
        canvas->left = left;
        canvas->top = top;
        canvas->right = right;
        canvas->bottom = bottom;
    }
    else
    {
        canvas->left = 0;
        canvas->top = 0;
        canvas->right = raster->width - 1;
        canvas->bottom = raster->height - 1;
    }
    canvas->center_x = (float)canvas->left + (float)(canvas->right - canvas->left + 1) / op_canvas_two;
    canvas->center_y = (float)canvas->top + (float)(canvas->bottom - canvas->top + 1) / op_canvas_two;
    return 1;
}
