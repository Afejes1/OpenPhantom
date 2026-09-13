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
int main(void)
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
                CHECK(p[i] == 0);
            CHECK(op_projection_dirty == dirty);
            CHECK(op_elapsed == 19.0f);
        }
    printf("projection clear: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
