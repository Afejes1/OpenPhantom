#include "../src/projected_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int po_expire_checks, po_expire_failures;
static void po_expire_check(int ok, int line)
{
    po_expire_checks++;
    if (!ok)
    {
        po_expire_failures++;
        printf("line %d failed\n", line);
    }
}
#define po_expire_CHECK(x) po_expire_check(!!(x), __LINE__)
static OP_PROJECTION po_expire_expected[255];
int po_expire_main(void)
{
    int s, l, po_expire_a, d, i;
    float lives[5] = {0.0f, -2.0f, 1.0f, 2.0f, 4.0f};
    float ages[7] = {-4.0f, -2.0f, 0.0f, 1.0f, 2.0f, 4.0f, 8.0f};
    int removed;
    for (s = 0; s < 255; s++)
        for (l = 0; l < 5; l++)
            for (po_expire_a = 0; po_expire_a < 7; po_expire_a++)
                for (d = -1; d <= 1; d++)
                {
                    memset(op_projections, 0x5A, sizeof(op_projections));
                    for (i = 0; i < 255; i++)
                    {
                        op_projections[i].surface = 0;
                        op_projections[i].created = 16.0f;
                        op_projections[i].lifetime = lives[l];
                    }
                    op_projections[s].surface = 0x80000000U;
                    op_elapsed = 16.0f + ages[po_expire_a];
                    op_projection_dirty = d;
                    memcpy(po_expire_expected, op_projections, sizeof(po_expire_expected));
                    removed = l != 0 && ages[po_expire_a] > lives[l];
                    if (removed)
                        memset(&po_expire_expected[s], 0, 68);
                    op_projection_expire();
                    for (i = 0; i < 255; i++)
                        po_expire_CHECK(memcmp(&op_projections[i], &po_expire_expected[i], 68) == 0);
                    po_expire_CHECK(op_projection_dirty == (removed ? 1 : d));
                    po_expire_CHECK(op_elapsed == 16.0f + ages[po_expire_a]);
                }
    memset(op_projections, 0x33, sizeof(op_projections));
    for (i = 0; i < 255; i++)
    {
        op_projections[i].surface = 1;
        op_projections[i].created = 0;
        op_projections[i].lifetime = 1;
    }
    op_elapsed = 2;
    op_projection_dirty = 0;
    op_projection_expire();
    memset(po_expire_expected, 0, sizeof(po_expire_expected));
    po_expire_CHECK(memcmp(po_expire_expected, op_projections, sizeof(po_expire_expected)) == 0);
    po_expire_CHECK(op_projection_dirty == 1);
    printf("projection expire: %d checks, %d failures\n", po_expire_checks, po_expire_failures);
    return po_expire_failures != 0;
}
