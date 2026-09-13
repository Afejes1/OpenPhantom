#include "line.h"
#include <stdlib.h>

#pragma intrinsic(abs)

/* FUNCTION: WMAIN 0x004084B8, bapline_reset (reported team name). */
op_scan_line *op_reset_scan_line(op_scan_line *line, int start_x, int start_y,
                                 int end_x, int end_y)
{
    int x_distance;
    int y_distance;

    if (line == 0) {
        line = (op_scan_line *)op_scan_allocate(sizeof(*line));
        if (line == 0)
            return line;
    }

    line->start_x = start_x;
    line->start_y = start_y;
    line->end_x = end_x;
    line->end_y = end_y;
    line->diagonal_x = 1;
    line->diagonal_y = 1;
    if (line->end_x < line->start_x)
        line->diagonal_x = -1;
    if (line->end_y < line->start_y)
        line->diagonal_y = -1;

    x_distance = abs(line->end_x - line->start_x);
    y_distance = abs(line->end_y - line->start_y);
    if (x_distance >= y_distance) {
        line->straight_x = line->diagonal_x;
        line->straight_y = 0;
        line->major_delta = x_distance;
        line->minor_delta = y_distance;
    } else {
        line->straight_x = 0;
        line->straight_y = line->diagonal_y;
        line->major_delta = y_distance;
        line->minor_delta = x_distance;
    }

    line->straight_error = line->minor_delta << 1;
    line->error = line->straight_error - line->major_delta;
    line->diagonal_error = line->error - line->major_delta;
    line->count = line->major_delta + 1;
    line->x = line->start_x;
    line->y = line->start_y;
    line->index = 0;
    line->step_x = 0;
    line->step_y = 0;
    return line;
}
