#include "api.h"
// FUNCTION: WMAIN 0x00460a30
void op_ui_get_cursor(int *x, int *y)
{
    *x = op_ui_cursor.x;
    *y = op_ui_cursor.y;
}
