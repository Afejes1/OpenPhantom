#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_RESOURCE_SLOT op_resource_slots[34];
static OP_RESOURCE_SLOT expected[34];
int main(void)
{
    int i, p, k, values[7] = {INT_MIN, -1, 0, 1, 32, 65537, INT_MAX};
    for (p = 0; p < 8; ++p)
        for (k = 0; k < 7; ++k)
        {
            memset(op_resource_slots, 0x20 + p, sizeof(op_resource_slots));
            for (i = 0; i < 34; ++i)
                op_resource_slots[i].backend_handle = values[(i + k) % 7];
            memcpy(expected, op_resource_slots, sizeof(expected));
            for (i = 0; i <= 32; ++i)
            {
                CHECK(op_resource_backend_handle(i) == values[(i + k) % 7]);
                CHECK(memcmp(op_resource_slots, expected, sizeof(expected)) == 0);
            }
        }
    printf("resource_backend_handle: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
