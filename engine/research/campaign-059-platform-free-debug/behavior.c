#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
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
static unsigned char owned[128], expected_owned[128];
static void *input, *output;
static unsigned int requested;
static int calls;
static void verify(void)
{
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void *op_platform_heap_alloc(unsigned int size)
{
    CHECK(calls++ == 0);
    CHECK(size == requested);
    verify();
    return output;
}
void op_platform_heap_free(void *ptr)
{
    CHECK(calls++ == 0);
    CHECK(ptr == input);
    verify();
}
void *op_platform_heap_realloc(void *ptr, unsigned int size)
{
    CHECK(calls++ == 0);
    CHECK(ptr == input);
    CHECK(size == requested);
    verify();
    return output;
}
int main(void)
{
    unsigned int i, j, k;
    static const unsigned int sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 9; ++j)
            for (k = 0; k < 9; ++k)
            {
                memset(owned, (int)(i + j + k), sizeof(owned));
                memcpy(expected_owned, owned, sizeof(owned));
                input = j ? owned + 7 * j : 0;
                output = k ? owned + 7 * k : 0;
                requested = sizes[i];
                calls = 0;
                op_platform_free_debug(input);
                CHECK(calls == 1);
                verify();
            }
    printf("platform_free_debug: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
