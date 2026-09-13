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

OP_HALO op_halos[32];
int op_halo_count;
typedef struct ACTOR
{
    unsigned int before;
    OP_ACTOR value;
    unsigned int after;
} ACTOR;
static ACTOR actors[2], expected_actors[2];
static OP_HALO expected[32];
static unsigned char owned[32][16], expected_owned[32][16];
static OP_ACTOR *wanted;
static int expected_count, calls, last_slot, dynamic;
static void verify_state(void)
{
    CHECK(memcmp(op_halos, expected, sizeof(expected)) == 0);
    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
    CHECK(op_halo_count == expected_count);
}
void op_release_sprite(void **sprite)
{
    int slot = last_slot + 1;
    while (slot < 32 && expected[slot].owner == 0)
        ++slot;
    CHECK(slot < 32);
    if (slot >= 32)
        return;
    CHECK(sprite == &op_halos[slot].sprite);
    expected[slot].owner = 0; /* Required clear before callback, not after it. */
    verify_state();
    ++calls;
    last_slot = slot;
    op_halos[slot].owner = &actors[1].value;
    expected[slot].owner = &actors[1].value;
    *sprite = calls % 2 ? 0 : owned[slot];
    expected[slot].sprite = *sprite;
    op_halo_count = 20 + slot;
    expected_count = 19 + slot;
    if (dynamic && slot == 0)
    {
        op_halos[1].owner = 0;
        expected[1].owner = 0;
        op_halos[2].owner = wanted;
        expected[2].owner = wanted;
    }
    if (dynamic && slot == 31)
    {
        op_halos[0].owner = wanted;
        expected[0].owner = wanted;
    }
    owned[slot][7] = 0x29;
    expected_owned[slot][7] = 0x29;
    actors[0].value.flags = 0x43U;
    expected_actors[0].value.flags = 0x43U;
}
static void run_case(int profile, int single)
{
    int i, total = 0;
    memset(op_halos, 0x36, sizeof(op_halos));
    memset(actors, 0x47, sizeof(actors));
    memset(owned, 0x58, sizeof(owned));
    wanted = &actors[0].value;
    dynamic = profile == 3;
    for (i = 0; i < 32; ++i)
    {
        op_halos[i].owner = 0;
        if (profile == 1 || (profile == 2 && i == single) || (profile == 3 && (i == 0 || i == 1 || i == 31)) ||
            (profile == 4 && i % 3 == 0))
        {
            op_halos[i].owner = wanted;
            ++total;
        }
        if (profile == 4 && i % 3 == 1)
            op_halos[i].owner = 0;
        op_halos[i].sprite = i % 2 ? owned[i] : 0;
    }
    memcpy(expected, op_halos, sizeof(expected));
    memcpy(expected_actors, actors, sizeof(actors));
    memcpy(expected_owned, owned, sizeof(owned));
    op_halo_count = -7;
    expected_count = -7;
    calls = 0;
    last_slot = -1;
    op_halo_free_all();
    expected_count = 0;
    CHECK(calls == total);
    verify_state();
}
int main(void)
{
    int i;
    run_case(0, 0);
    run_case(1, 0);
    for (i = 0; i < 32; ++i)
        run_case(2, i);
    run_case(3, 0);
    run_case(4, 0);
    printf("halo free all: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
