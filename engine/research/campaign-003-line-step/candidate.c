#include "api.h"

/* FUNCTION: WMAIN 0x004083D0, bapline_step (reported team name). */
int op_step_scan_line(op_scan_line *line)
{
    if (line->error >= 0) {
        line->step_x = line->diagonal_x;
        line->step_y = line->diagonal_y;
        line->x += line->diagonal_x;
        line->y += line->diagonal_y;
        line->error += line->diagonal_error;
    } else {
        line->step_x = line->straight_x;
        line->step_y = line->straight_y;
        line->x += line->straight_x;
        line->y += line->straight_y;
        line->error += line->straight_error;
    }
    ++line->index;
    if (line->index > line->count)
        return 1;
    return 0;
}
