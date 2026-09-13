#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

static unsigned char owned[16], expected[16];
static int calls;
static void *wanted;
void op_release(void *memory)
{
    CHECK(calls++ == 0);
    CHECK(memory == wanted);
    CHECK(memcmp(owned, expected, sizeof(owned)) == 0);
    owned[3] = 0x77;
    expected[3] = 0x77;
}
int main(void)
{
    int i;
    memset(owned, 0x42, sizeof(owned));
    memcpy(expected, owned, sizeof(owned));
    calls = 0;
    op_shield_free(0);
    CHECK(calls == 0);
    CHECK(memcmp(owned, expected, sizeof(owned)) == 0);
    for (i = 0; i < 2; ++i)
    {
        calls = 0;
        wanted = owned + i;
        op_shield_free(wanted);
        CHECK(calls == 1);
        CHECK(memcmp(owned, expected, sizeof(owned)) == 0);
    }
    printf("shield free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
