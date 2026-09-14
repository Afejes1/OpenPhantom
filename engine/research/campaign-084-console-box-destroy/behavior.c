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
static unsigned char bytes[8];
static OP_CONSOLE_BOX *input;
static void *first;
static int calls, mutate;
void op_platform_heap_free(void *ptr)
{
    CHECK(calls < 2);
    if (calls >= 2)
        exit(2);
    CHECK(ptr == (calls == 0 ? first : input));
    CHECK(memcmp(boxes, expected_boxes, sizeof(boxes)) == 0);
    if (mutate)
    {
        input->buffer = &bytes[7];
        expected_boxes[input == &boxes[1].box].box.buffer = &bytes[7];
        input->tail[3] = 0x71;
        expected_boxes[input == &boxes[1].box].box.tail[3] = 0x71;
    }
    ++calls;
}
int main(void)
{
    int a, b, p;
    for (a = 0; a < 2; ++a)
        for (b = 0; b < 4; ++b)
            for (p = 0; p < 8; ++p)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    memset(boxes, 0x31 + p, sizeof(boxes));
                    input = &boxes[a].box;
                    first = b == 0   ? 0
                            : b == 1 ? (void *)&bytes[1]
                            : b == 2 ? (void *)input
                                     : (void *)&boxes[1 - a].box;
                    input->buffer = first;
                    memcpy(expected_boxes, boxes, sizeof(boxes));
                    calls = 0;
                    op_console_box_destroy(input);
                    CHECK(calls == 2);
                    CHECK(memcmp(boxes, expected_boxes, sizeof(boxes)) == 0);
                }
    printf("console_box_destroy: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
