#include "segment.h"
#include <stdio.h>

static int checks;
static int failures;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static int determinant(int ax, int ay, int bx, int by, int px, int py)
{
    return (bx - ax) * (py - ay) - (by - ay) * (px - ax);
}

static int oracle(int x1, int y1, int x2, int y2,
                  int x3, int y3, int x4, int y4)
{
    int d3 = determinant(x1, y1, x2, y2, x3, y3);
    int d4 = determinant(x1, y1, x2, y2, x4, y4);
    int d1 = determinant(x3, y3, x4, y4, x1, y1);
    int d2 = determinant(x3, y3, x4, y4, x2, y2);

    if (d3 != 0 && d4 != 0 && ((d3 < 0) == (d4 < 0)))
        return 0;
    if (d1 != 0 && d2 != 0 && ((d1 < 0) == (d2 < 0)))
        return 0;
    return 1;
}

static void explicit_case(int x1, int y1, int x2, int y2,
                          int x3, int y3, int x4, int y4,
                          int expected, const char *message)
{
    int actual = op_segments_intersect(x1, y1, x2, y2,
                                       x3, y3, x4, y4);
    check(actual == expected, message);
    check(op_segments_intersect(x2, y2, x1, y1,
                                x3, y3, x4, y4) == expected,
          "first endpoint reversal");
    check(op_segments_intersect(x1, y1, x2, y2,
                                x4, y4, x3, y3) == expected,
          "second endpoint reversal");
    check(op_segments_intersect(x3, y3, x4, y4,
                                x1, y1, x2, y2) == expected,
          "segment swapping");
}

int main(void)
{
    int x1, y1, x2, y2, x3, y3, x4, y4;

    explicit_case(0, 0, 4, 4, 0, 4, 4, 0, 1, "crossing");
    explicit_case(0, 0, 4, 0, 0, 1, 4, 1, 0, "parallel separation");
    explicit_case(0, 0, 4, 0, 5, 1, 7, 2, 0, "first-pair rejection");
    explicit_case(0, 0, 2, 0, 3, -1, 3, 1, 0, "second-pair rejection");
    explicit_case(0, 0, 4, 0, 4, 0, 5, 3, 1, "endpoint contact");
    explicit_case(0, 0, 4, 0, 2, 0, 6, 0, 1, "collinear overlap");
    explicit_case(0, 0, 1, 0, 3, 0, 4, 0, 1, "disjoint collinear");
    explicit_case(0, 0, 0, 0, 3, 3, 3, 3, 1, "distinct zero-length");
    explicit_case(2, 2, 2, 2, 0, 0, 4, 4, 1, "point on supporting line");
    explicit_case(2, 3, 2, 3, 0, 0, 4, 4, 0, "point off supporting line");

    for (x1 = -1; x1 <= 1; ++x1)
    for (y1 = -1; y1 <= 1; ++y1)
    for (x2 = -1; x2 <= 1; ++x2)
    for (y2 = -1; y2 <= 1; ++y2)
    for (x3 = -1; x3 <= 1; ++x3)
    for (y3 = -1; y3 <= 1; ++y3)
    for (x4 = -1; x4 <= 1; ++x4)
    for (y4 = -1; y4 <= 1; ++y4)
        check(op_segments_intersect(x1, y1, x2, y2,
                                    x3, y3, x4, y4) ==
              oracle(x1, y1, x2, y2, x3, y3, x4, y4),
              "small-coordinate determinant property");

    printf("Segment test: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
