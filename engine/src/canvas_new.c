#include "canvas.h"
// FUNCTION: WMAIN 0x004768d0
OP_CANVAS *op_canvas_new(unsigned int flags, OP_RASTER *raster, unsigned int field10, int left, int top, int right,
                         int bottom, unsigned int field14)
{
    OP_CANVAS *canvas;
    canvas = (OP_CANVAS *)op_allocate(sizeof(OP_CANVAS));
    if (!canvas)
        return 0;
    op_canvas_init(canvas, flags, raster, field10, left, top, right, bottom, field14);
    return canvas;
}
