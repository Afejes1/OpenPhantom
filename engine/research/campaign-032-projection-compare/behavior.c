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
static OP_PROJECTION a, b, saved_a, saved_b;
int main(void)
{
    unsigned int words[7] = {0, 1, 2, 0x7FFFFFFFU, 0x80000000U, 0xFFFFFFFEU, 0xFFFFFFFFU};
    int i, j, k, l, want, got;
    for (i = 0; i < 7; i++)
        for (j = 0; j < 7; j++)
            for (k = 0; k < 7; k++)
                for (l = 0; l < 7; l++)
                {
                    memset(&a, 0xA5, sizeof(a));
                    memset(&b, 0x5A, sizeof(b));
                    a.surface = words[i];
                    b.surface = words[j];
                    a.sequence = words[k];
                    b.sequence = words[l];
                    saved_a = a;
                    saved_b = b;
                    want = i < j ? -1 : (i > j ? 1 : (k < l ? -1 : (k > l ? 1 : 0)));
                    got = op_projection_compare(&a, &b);
                    CHECK(got == want);
                    CHECK(op_projection_compare(&b, &a) == -want);
                    CHECK(op_projection_compare(&a, &a) == 0);
                    CHECK(memcmp(&a, &saved_a, sizeof(a)) == 0);
                    CHECK(memcmp(&b, &saved_b, sizeof(b)) == 0);
                }
    printf("projection compare: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
