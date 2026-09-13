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
int main(void)
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
                memcpy(expected, op_projections, sizeof(expected));
                if (tokens[t] != 0)
                {
                    memset(&expected[s], 0, 68);
                    memset(&expected[(s + 19) % 255], 0, 68);
                }
                op_projection_dirty = d;
                op_elapsed = 77.0f;
                op_projection_remove(tokens[t]);
                for (i = 0; i < 255; i++)
                    CHECK(memcmp(&op_projections[i], &expected[i], 68) == 0);
                CHECK(op_projection_dirty == (tokens[t] ? 1 : d));
                CHECK(op_elapsed == 77.0f);
            }
    memset(op_projections, 0, sizeof(op_projections));
    op_projection_dirty = -9;
    op_projection_remove(42);
    CHECK(op_projection_dirty == -9);
    printf("projection remove: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
