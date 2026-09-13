#include "segment.h"

/* FUNCTION: WMAIN 0x00408648, bapline_segmentsIntersect (reported team name). */
int op_segments_intersect(int x1, int y1, int x2, int y2,
                          int x3, int y3, int x4, int y4)
{
    int a1;
    int b1;
    int c1;
    int a2;
    int b2;
    int c2;
    int r1;
    int r2;
    int r3;
    int r4;

    a1 = y2 - y1;
    b1 = x1 - x2;
    c1 = x2 * y1 - x1 * y2;
    r3 = a1 * x3 + b1 * y3 + c1;
    r4 = a1 * x4 + b1 * y4 + c1;
    if (r3 != 0 && r4 != 0 && (r3 ^ r4) >= 0)
        return 0;

    a2 = y4 - y3;
    b2 = x3 - x4;
    c2 = x4 * y3 - x3 * y4;
    r1 = a2 * x1 + b2 * y1 + c2;
    r2 = a2 * x2 + b2 * y2 + c2;
    if (r1 != 0 && r2 != 0 && (r1 ^ r2) >= 0)
        return 0;
    return 1;
}
