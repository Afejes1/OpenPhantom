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
OP_ZAP op_zaps[64];
static OP_ZAP expected[64];
typedef struct ACTORS
{
    unsigned int before;
    OP_ACTOR data[2];
    unsigned int after;
} ACTORS;
static ACTORS actors, expected_actors;
static unsigned char side[24], expected_side[24];
static void initialize(void)
{
    int i, j;
    memset(op_zaps, 0x39, sizeof(op_zaps));
    memset(&actors, 0x58, sizeof(actors));
    memset(side, 0x67, sizeof(side));
    for (i = 0; i < 64; ++i)
    {
        op_zaps[i].active = 0;
        for (j = 0; j < 4; ++j)
            op_zaps[i].endpoints[j] = &actors.data[1];
    }
    expected_actors = actors;
    memcpy(expected_side, side, sizeof(side));
}
static void verify_state(void)
{
    CHECK(memcmp(op_zaps, expected, sizeof(op_zaps)) == 0);
    CHECK(memcmp(&actors, &expected_actors, sizeof(actors)) == 0);
    CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
}
static void single(int slot, int endpoint, int active, int null_actor)
{
    OP_ACTOR *wanted;
    initialize();
    wanted = null_actor ? 0 : &actors.data[0];
    op_zaps[slot].active = active;
    op_zaps[slot].endpoints[endpoint] = wanted;
    memcpy(expected, op_zaps, sizeof(expected));
    if (active)
        expected[slot].endpoints[endpoint] = 0;
    op_detach_zap(wanted);
    verify_state();
}
static void multiple(int inactive)
{
    int i, j;
    initialize();
    for (i = 0; i < 64; ++i)
    {
        op_zaps[i].active = inactive ? 0 : (i % 2 ? INT_MIN : 1);
        for (j = 0; j < 4; ++j)
            op_zaps[i].endpoints[j] = &actors.data[0];
    }
    memcpy(expected, op_zaps, sizeof(expected));
    if (!inactive)
        for (i = 0; i < 64; ++i)
            for (j = 0; j < 4; ++j)
                expected[i].endpoints[j] = 0;
    op_detach_zap(&actors.data[0]);
    verify_state();
}
int main(void)
{
    static const int active[3] = {0, 1, INT_MIN};
    int i, j, k, n;
    for (i = 0; i < 64; ++i)
        for (j = 0; j < 4; ++j)
            for (k = 0; k < 3; ++k)
                for (n = 0; n < 2; ++n)
                    single(i, j, active[k], n);
    multiple(0);
    multiple(1);
    printf("zap endpoint detach: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
