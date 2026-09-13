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
static void single(int slot, int endpoint, int active, int mode, int null_actor)
{
    int i, j, result;
    OP_ACTOR *wanted;
    memset(op_zaps, 0x49, sizeof(op_zaps));
    memset(&actors, 0x58, sizeof(actors));
    memset(side, 0x67, sizeof(side));
    for (i = 0; i < 64; ++i)
    {
        op_zaps[i].active = 0;
        op_zaps[i].mode = 0;
        for (j = 0; j < 4; ++j)
            op_zaps[i].endpoints[j] = &actors.data[1];
    }
    wanted = null_actor ? 0 : &actors.data[0];
    op_zaps[slot].active = active;
    op_zaps[slot].mode = mode;
    op_zaps[slot].endpoints[endpoint] = wanted;
    memcpy(expected, op_zaps, sizeof(expected));
    expected_actors = actors;
    memcpy(expected_side, side, sizeof(side));
    result = op_zap_has_actor(wanted);
    CHECK(result == ((active != 0) && (mode != 0)));
    CHECK(memcmp(op_zaps, expected, sizeof(expected)) == 0);
    CHECK(memcmp(&actors, &expected_actors, sizeof(actors)) == 0);
    CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
}
int main(void)
{
    static const int values[3] = {0, 1, INT_MIN};
    int i, j, a, m, n;
    for (i = 0; i < 64; ++i)
        for (j = 0; j < 4; ++j)
            for (a = 0; a < 3; ++a)
                for (m = 0; m < 3; ++m)
                    for (n = 0; n < 2; ++n)
                        single(i, j, values[a], values[m], n);
    /* Empty pool: endpoint words may be NULL, but no active record qualifies. */
    memset(op_zaps, 0, sizeof(op_zaps));
    memcpy(expected, op_zaps, sizeof(expected));
    CHECK(op_zap_has_actor(0) == 0);
    CHECK(op_zap_has_actor(&actors.data[0]) == 0);
    CHECK(memcmp(op_zaps, expected, sizeof(expected)) == 0);
    printf("zap actor query: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
