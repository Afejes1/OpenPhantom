#include "../src/projected_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int po_clear_checks, po_clear_failures;
static void po_clear_check(int ok, int line)
{
    po_clear_checks++;
    if (!ok)
    {
        po_clear_failures++;
        printf("line %d failed\n", line);
    }
}
#define po_clear_CHECK(x) po_clear_check(!!(x), __LINE__)
int po_clear_main(void)
{
    int fill, dirty, i;
    unsigned char *p = (unsigned char *)op_projections;
    for (fill = 0; fill < 256; fill += 17)
        for (dirty = -1; dirty <= 1; dirty++)
        {
            memset(op_projections, fill, sizeof(op_projections));
            op_projection_dirty = dirty;
            op_elapsed = 19.0f;
            op_projection_clear();
            for (i = 0; i < (int)sizeof(op_projections); i++)
                po_clear_CHECK(p[i] == 0);
            po_clear_CHECK(op_projection_dirty == dirty);
            po_clear_CHECK(op_elapsed == 19.0f);
        }
    printf("projection clear: %d checks, %d failures\n", po_clear_checks, po_clear_failures);
    return po_clear_failures != 0;
}
