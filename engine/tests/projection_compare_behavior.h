#include "../src/projected_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int po_compare_checks, po_compare_failures;
static void po_compare_check(int ok, int line)
{
    po_compare_checks++;
    if (!ok)
    {
        po_compare_failures++;
        printf("line %d failed\n", line);
    }
}
#define po_compare_CHECK(x) po_compare_check(!!(x), __LINE__)
static OP_PROJECTION po_compare_a, po_compare_b, po_compare_saved_a, po_compare_saved_b;
int po_compare_main(void)
{
    unsigned int words[7] = {0, 1, 2, 0x7FFFFFFFU, 0x80000000U, 0xFFFFFFFEU, 0xFFFFFFFFU};
    int i, j, k, l, want, got;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            for (k = 0; k < 7; k++)
                for (l = 0; l < 7; l++)
                {
                    memset(&po_compare_a, 0xA5, sizeof(po_compare_a));
                    memset(&po_compare_b, 0x5A, sizeof(po_compare_b));
                    po_compare_a.surface = words[i];
                    po_compare_b.surface = words[j];
                    po_compare_a.sequence = words[k];
                    po_compare_b.sequence = words[l];
                    po_compare_saved_a = po_compare_a;
                    po_compare_saved_b = po_compare_b;
                    want = i < j ? -1 : (i > j ? 1 : (k < l ? -1 : (k > l ? 1 : 0)));
                    got = op_projection_compare(&po_compare_a, &po_compare_b);
                    po_compare_CHECK(got == want);
                    po_compare_CHECK(op_projection_compare(&po_compare_b, &po_compare_a) == -want);
                    po_compare_CHECK(op_projection_compare(&po_compare_a, &po_compare_a) == 0);
                    po_compare_CHECK(memcmp(&po_compare_a, &po_compare_saved_a, sizeof(po_compare_a)) == 0);
                    po_compare_CHECK(memcmp(&po_compare_b, &po_compare_saved_b, sizeof(po_compare_b)) == 0);
                }
    printf("projection compare: %d checks, %d failures\n", po_compare_checks, po_compare_failures);
    return po_compare_failures != 0;
}
