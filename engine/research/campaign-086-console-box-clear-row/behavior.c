#include "api.h"
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)

typedef struct OWNED_BOX
{
    unsigned int before;
    OP_CONSOLE_BOX box;
    unsigned int after;
} OWNED_BOX;
static OWNED_BOX boxes[2], expected_boxes[2];
static unsigned char bytes[288], expected_bytes[288];
int main(void)
{
    int a, n, r, p, i;
    unsigned char *expected_destination;
    for (a = 0; a < 2; ++a)
        for (n = 0; n < 32; ++n)
            for (r = -3; r <= 3; ++r)
                for (p = 0; p < 4; ++p)
                {
                    memset(boxes, 0x31 + p, sizeof(boxes));
                    memset(bytes, 0x61 + p, sizeof(bytes));
                    boxes[a].box.buffer = &bytes[128 + p];
                    boxes[a].box.width = n + 2;
                    memcpy(expected_boxes, boxes, sizeof(boxes));
                    memcpy(expected_bytes, bytes, sizeof(bytes));
                    expected_destination = &expected_bytes[128 + p + r * n];
                    for (i = 0; i < n; ++i)
                        expected_destination[i] = ' ';
                    op_console_box_clear_row(&boxes[a].box, r);
                    CHECK(memcmp(boxes, expected_boxes, sizeof(boxes)) == 0);
                    CHECK(memcmp(bytes, expected_bytes, sizeof(bytes)) == 0);
                }
    printf("console_box_clear_row: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
