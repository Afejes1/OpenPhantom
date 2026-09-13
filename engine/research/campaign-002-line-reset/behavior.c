#include "line.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

static op_scan_line allocation;
static int allocation_calls;
static unsigned int allocation_size;
static int allocation_fails;
static int checks;
static int failures;

void *op_scan_allocate(unsigned int size)
{
    ++allocation_calls;
    allocation_size = size;
    if (allocation_fails)
        return 0;
    return &allocation;
}

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static int magnitude(int value)
{
    return value < 0 ? -value : value;
}

static void run_case(int start_x, int start_y, int end_x, int end_y)
{
    op_scan_line line;
    int dx = magnitude(end_x - start_x);
    int dy = magnitude(end_y - start_y);
    int x_sign = end_x < start_x ? -1 : 1;
    int y_sign = end_y < start_y ? -1 : 1;
    int major = dx >= dy ? dx : dy;
    int minor = dx >= dy ? dy : dx;

    memset(&line, 0x5a, sizeof(line));
    allocation_calls = 0;
    check(op_reset_scan_line(&line, start_x, start_y, end_x, end_y) == &line,
          "provided record return");
    check(allocation_calls == 0, "provided record does not allocate");
    check(line.count == major + 1, "count");
    check(line.index == 0, "index");
    check(line.x == start_x && line.y == start_y, "current position");
    check(line.step_x == 0 && line.step_y == 0, "current step reset");
    check(line.start_x == start_x && line.start_y == start_y, "start fields");
    check(line.end_x == end_x && line.end_y == end_y, "end fields");
    check(line.major_delta == major && line.minor_delta == minor, "deltas");
    check(line.error == 2 * minor - major, "current error");
    check(line.diagonal_x == x_sign && line.diagonal_y == y_sign,
          "diagonal signs");
    check(line.diagonal_error == 2 * minor - 2 * major,
          "diagonal error");
    check(line.straight_x == (dx >= dy ? x_sign : 0) &&
          line.straight_y == (dx >= dy ? 0 : y_sign), "straight step");
    check(line.straight_error == 2 * minor, "straight error");
    check(line.unused_4c == 0x5a5a5a5a, "unused word preserved");
}

int main(void)
{
    op_scan_line *result;
    static const int cases[][4] = {
        {0, 0, 5, 2}, {0, 0, -5, 2}, {0, 0, 5, -2},
        {0, 0, -5, -2}, {3, 4, 5, 10}, {3, 4, 1, 10},
        {3, 4, 5, -2}, {3, 4, 1, -2}, {7, -3, 11, 1},
        {7, -3, 3, -7}, {9, 9, 9, 9},
        {0, 0, 5, 0}, {0, 0, -5, 0},
        {0, 0, 0, 5}, {0, 0, 0, -5}
    };
    unsigned int i;
    unsigned char untouched[sizeof(allocation)];

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
        run_case(cases[i][0], cases[i][1], cases[i][2], cases[i][3]);

    memset(&allocation, 0x6b, sizeof(allocation));
    allocation_calls = 0;
    allocation_size = 0;
    allocation_fails = 0;
    result = op_reset_scan_line(0, -2, 8, 4, -1);
    check(result == &allocation, "allocation success return");
    check(allocation_calls == 1 && allocation_size == 80,
          "allocation request is one 80-byte record");
    check(allocation.unused_4c == 0x6b6b6b6b,
          "allocated record unused word preserved");
    check(allocation.count == 10 && allocation.index == 0,
          "allocated record count and index");
    check(allocation.x == -2 && allocation.y == 8,
          "allocated record current position");
    check(allocation.step_x == 0 && allocation.step_y == 0,
          "allocated record current step");
    check(allocation.start_x == -2 && allocation.start_y == 8 &&
          allocation.end_x == 4 && allocation.end_y == -1,
          "allocated record endpoints");
    check(allocation.major_delta == 9 && allocation.minor_delta == 6,
          "allocated record deltas");
    check(allocation.error == 3 && allocation.diagonal_error == -6 &&
          allocation.straight_error == 12, "allocated record errors");
    check(allocation.diagonal_x == 1 && allocation.diagonal_y == -1 &&
          allocation.straight_x == 0 && allocation.straight_y == -1,
          "allocated record steps");

    memset(&allocation, 0x7c, sizeof(allocation));
    memset(untouched, 0x7c, sizeof(untouched));
    allocation_calls = 0;
    allocation_size = 0;
    allocation_fails = 1;
    result = op_reset_scan_line(0, 1, 2, 3, 4);
    check(result == 0, "allocation failure return");
    check(allocation_calls == 1 && allocation_size == 80,
          "allocation failure request");
    check(memcmp(&allocation, untouched, sizeof(allocation)) == 0,
          "allocation failure preserves all 80 record bytes");

    printf("Line reset: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
