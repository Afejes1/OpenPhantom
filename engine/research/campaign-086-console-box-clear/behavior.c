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
    int a, alias, n, p, i;
    unsigned char *expected_destination;
    for (a = 0; a < 2; ++a)
        for (alias = 0; alias < 2; ++alias)
            for (n = 0; n < 49; ++n)
                for (p = 0; p < 4; ++p)
                {
                    memset(boxes, 0x31 + p, sizeof(boxes));
                    memset(bytes, 0x61 + p, sizeof(bytes));
                    boxes[a].box.buffer = alias ? (void *)&boxes[a].box : (void *)&bytes[32 + p];
                    boxes[a].box.inner_size = (unsigned int)n;
                    memcpy(expected_boxes, boxes, sizeof(boxes));
                    memcpy(expected_bytes, bytes, sizeof(bytes));
                    expected_destination = alias ? (unsigned char *)&expected_boxes[a].box : &expected_bytes[32 + p];
                    for (i = 0; i < n; ++i)
                        expected_destination[i] = ' ';
                    expected_boxes[a].box.column = 0;
                    expected_boxes[a].box.row = 0;
                    op_console_box_clear(&boxes[a].box);
                    CHECK(memcmp(boxes, expected_boxes, sizeof(boxes)) == 0);
                    CHECK(memcmp(bytes, expected_bytes, sizeof(bytes)) == 0);
                }
    printf("console_box_clear: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
