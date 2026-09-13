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
const float op_projection_zero = 0.0f;
static OP_PROJECTION expected[255];
int main(void)
{
    int s, l, a, d, i;
    float lives[5] = {0.0f, -2.0f, 1.0f, 2.0f, 4.0f};
    float ages[7] = {-4.0f, -2.0f, 0.0f, 1.0f, 2.0f, 4.0f, 8.0f};
    int removed;
    for (s = 0; s < 255; s++)
        for (l = 0; l < 5; l++)
            for (a = 0; a < 7; a++)
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
                    op_elapsed = 16.0f + ages[a];
                    op_projection_dirty = d;
                    memcpy(expected, op_projections, sizeof(expected));
                    removed = l != 0 && ages[a] > lives[l];
                    if (removed)
                        memset(&expected[s], 0, 68);
                    op_projection_expire();
                    for (i = 0; i < 255; i++)
                        CHECK(memcmp(&op_projections[i], &expected[i], 68) == 0);
                    CHECK(op_projection_dirty == (removed ? 1 : d));
                    CHECK(op_elapsed == 16.0f + ages[a]);
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
    memset(expected, 0, sizeof(expected));
    CHECK(memcmp(expected, op_projections, sizeof(expected)) == 0);
    CHECK(op_projection_dirty == 1);
    printf("projection expire: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
