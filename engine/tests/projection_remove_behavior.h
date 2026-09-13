#include "../src/projected_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int po_remove_checks, po_remove_failures;
static void po_remove_check(int ok, int line)
{
    po_remove_checks++;
    if (!ok)
    {
        po_remove_failures++;
        printf("line %d failed\n", line);
    }
}
#define po_remove_CHECK(x) po_remove_check(!!(x), __LINE__)
static OP_PROJECTION po_remove_expected[255];
int po_remove_main(void)
{
    int t, s, d, i;
    int tokens[6] = {0, 1, -1, INT_MIN, INT_MAX, 37};
    for (t = 0; t < 6; t++)
        for (s = 0; s < 255; s++)
            for (d = -1; d <= 1; d++)
            {
                memset(op_projections, 0xA5, sizeof(op_projections));
                for (i = 0; i < 255; i++)
                {
                    op_projections[i].group_token = 42;
                    op_projections[i].surface = (i & 1) ? 0U : 0xFFFFFFFFU;
                }
                op_projections[s].group_token = tokens[t];
                op_projections[(s + 19) % 255].group_token = tokens[t];
                memcpy(po_remove_expected, op_projections, sizeof(po_remove_expected));
                if (tokens[t] != 0)
                {
                    memset(&po_remove_expected[s], 0, 68);
                    memset(&po_remove_expected[(s + 19) % 255], 0, 68);
                }
                op_projection_dirty = d;
                op_elapsed = 77.0f;
                op_projection_remove(tokens[t]);
                for (i = 0; i < 255; i++)
                    po_remove_CHECK(memcmp(&op_projections[i], &po_remove_expected[i], 68) == 0);
                po_remove_CHECK(op_projection_dirty == (tokens[t] ? 1 : d));
                po_remove_CHECK(op_elapsed == 77.0f);
            }
    memset(op_projections, 0, sizeof(op_projections));
    op_projection_dirty = -9;
    op_projection_remove(42);
    po_remove_CHECK(op_projection_dirty == -9);
    printf("projection remove: %d checks, %d failures\n", po_remove_checks, po_remove_failures);
    return po_remove_failures != 0;
}
