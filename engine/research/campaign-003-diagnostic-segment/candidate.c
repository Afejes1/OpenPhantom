#include "api.h"

/* FUNCTION: WMAIN 0x00408913 */
void op_draw_diagnostic_segment(const op_point3 *first, const op_point3 *second)
{
    op_point3 segment[2];
    segment[0] = *first;
    segment[1] = *second;
    op_submit_polyline((const float *)segment, 2, 0xff00ff00, 0, 8.0f);
}