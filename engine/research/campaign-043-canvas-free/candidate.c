#include "api.h"
// FUNCTION: WMAIN 0x00476a04
void op_canvas_free(OP_CANVAS *canvas)
{
    if (canvas)
    {
        op_canvas_free_inner(canvas);
        op_release(canvas);
    }
}
