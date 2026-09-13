#include "../src/projected_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int po_sort_checks, po_sort_failures;
static void po_sort_check(int ok, int line)
{
    po_sort_checks++;
    if (!ok)
    {
        po_sort_failures++;
        printf("line %d failed\n", line);
    }
}
#define po_sort_CHECK(x) po_sort_check(!!(x), __LINE__)
static OP_PROJECTION po_sort_expected[255];
static int po_sort_calls, po_sort_before_dirty;
void po_sort_observer(void *p, unsigned int n, unsigned int size, int (*compare)(const void *, const void *))
{
    po_sort_CHECK(p == op_projections);
    po_sort_CHECK(n == 255);
    po_sort_CHECK(size == 68);
    po_sort_CHECK(compare == op_projection_compare);
    po_sort_CHECK(op_projection_dirty == po_sort_before_dirty);
    po_sort_CHECK(memcmp(p, po_sort_expected, sizeof(po_sort_expected)) == 0);
    po_sort_calls++;
    op_projections[0].surface = 123;
    op_projections[254].sequence = 456;
    po_sort_expected[0].surface = 123;
    po_sort_expected[254].sequence = 456;
    op_projection_dirty = 19;
    op_elapsed = 27.0f;
}
int po_sort_main(void)
{
    int d, i;
    int states[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (d = 0; d < 5; d++)
    {
        memset(op_projections, 0x6B, sizeof(op_projections));
        memcpy(po_sort_expected, op_projections, sizeof(po_sort_expected));
        op_projection_dirty = states[d];
        po_sort_before_dirty = states[d];
        po_sort_calls = 0;
        op_elapsed = 3.0f;
        op_projection_sort();
        po_sort_CHECK(po_sort_calls == (states[d] != 0));
        po_sort_CHECK(op_projection_dirty == 0);
        for (i = 0; i < 255; i++)
            po_sort_CHECK(memcmp(&po_sort_expected[i], &op_projections[i], 68) == 0);
        po_sort_CHECK(op_elapsed == (states[d] ? 27.0f : 3.0f));
        op_projection_sort();
        po_sort_CHECK(po_sort_calls == (states[d] != 0));
        po_sort_CHECK(op_projection_dirty == 0);
    }
    printf("projection sort: %d checks, %d failures\n", po_sort_checks, po_sort_failures);
    return po_sort_failures != 0;
}
