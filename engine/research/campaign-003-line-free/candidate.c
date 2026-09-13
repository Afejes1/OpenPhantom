#include "api.h"

/* FUNCTION: WMAIN 0x004084A1, bapline_free (reported team name). */
void op_free_scan_line(op_scan_line *line)
{
    if (line != 0)
        op_scan_release(line);
}
