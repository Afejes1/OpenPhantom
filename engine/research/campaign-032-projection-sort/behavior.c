#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_PROJECTION op_projections[255];
int op_projection_dirty;
float op_elapsed;
static OP_PROJECTION expected[255];
static int calls, before_dirty;
int op_projection_compare(const void *a, const void *b)
{
    return a == b ? 0 : 1;
}
void op_projection_qsort(void *p, unsigned int n, unsigned int size, int (*compare)(const void *, const void *))
{
    CHECK(p == op_projections);
    CHECK(n == 255);
    CHECK(size == 68);
    CHECK(compare == op_projection_compare);
    CHECK(op_projection_dirty == before_dirty);
    CHECK(memcmp(p, expected, sizeof(expected)) == 0);
    calls++;
    op_projections[0].surface = 123;
    op_projections[254].sequence = 456;
    expected[0].surface = 123;
    expected[254].sequence = 456;
    op_projection_dirty = 19;
    op_elapsed = 27.0f;
}
int main(void)
{
    int d, i;
    int states[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (d = 0; d < 5; d++)
    {
        memset(op_projections, 0x6B, sizeof(op_projections));
        memcpy(expected, op_projections, sizeof(expected));
        op_projection_dirty = states[d];
        before_dirty = states[d];
        calls = 0;
        op_elapsed = 3.0f;
        op_projection_sort();
        CHECK(calls == (states[d] != 0));
        CHECK(op_projection_dirty == 0);
        for (i = 0; i < 255; i++)
            CHECK(memcmp(&expected[i], &op_projections[i], 68) == 0);
        CHECK(op_elapsed == (states[d] ? 27.0f : 3.0f));
        op_projection_sort();
        CHECK(calls == (states[d] != 0));
        CHECK(op_projection_dirty == 0);
    }
    printf("projection sort: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
