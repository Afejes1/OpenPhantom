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
static OP_HALO expected[32];
typedef struct ACTORS
{
    unsigned int before;
    OP_ACTOR value[2];
    unsigned int after;
} ACTORS;
static ACTORS actors, expected_actors;
static unsigned char owned[32][16], expected_owned[32][16];
static OP_ACTOR *wanted;
static int calls, last_slot, dynamic;
static void verify_state(void)
{
    CHECK(memcmp(op_halos, expected, sizeof(expected)) == 0);
    CHECK(memcmp(&actors, &expected_actors, sizeof(actors)) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void op_halo_draw_slot(OP_ACTOR *actor, int slot)
{
    int next = last_slot + 1;
    while (next < 32 && expected[next].owner != wanted)
        ++next;
    CHECK(slot == next && next < 32);
    CHECK(actor == wanted);
    verify_state();
    if (next >= 32)
        return;
    ++calls;
    last_slot = next;
    /* Entry gate only: clearing bit0x10 must not stop the remaining scan. */
    actor->flags = 0;
    expected_actors.value[0].flags = 0;
    op_halos[slot].sprite = 0;
    expected[slot].sprite = 0;
    owned[slot][7] = 0x29;
    expected_owned[slot][7] = 0x29;
    if (dynamic && slot == 0)
    {
        op_halos[1].owner = &actors.value[1];
        expected[1].owner = &actors.value[1];
        op_halos[2].owner = wanted;
        expected[2].owner = wanted;
    }
    if (dynamic && slot == 31)
    {
        op_halos[0].owner = wanted;
        expected[0].owner = wanted;
    }
}
static void run_case(int profile, int single, unsigned int flags, int null_actor)
{
    int i, total = 0;
    memset(op_halos, 0x35, sizeof(op_halos));
    memset(&actors, 0x46, sizeof(actors));
    memset(owned, 0x57, sizeof(owned));
    actors.value[0].flags = flags;
    wanted = null_actor ? 0 : &actors.value[0];
    dynamic = profile == 3;
    calls = 0;
    last_slot = -1;
    for (i = 0; i < 32; ++i)
    {
        op_halos[i].owner = &actors.value[1];
        op_halos[i].sprite = owned[i];
        if (profile == 1 || (profile == 2 && i == single) || (profile == 3 && (i == 0 || i == 1 || i == 31)))
        {
            op_halos[i].owner = wanted;
            ++total;
        }
    }
    memcpy(expected, op_halos, sizeof(expected));
    expected_actors = actors;
    memcpy(expected_owned, owned, sizeof(owned));
    op_halo_draw_actor(wanted);
    CHECK(calls == (!null_actor && (flags & 0x10U) ? total : 0));
    verify_state();
}
int main(void)
{
    int i;
    run_case(1, 0, 0, 0);
    run_case(1, 0, 1, 0);
    run_case(1, 0, 0x10, 1);
    run_case(0, 0, 0x10, 0);
    run_case(1, 0, 0x10, 0);
    run_case(1, 0, 0xffffffffU, 0);
    for (i = 0; i < 32; ++i)
        run_case(2, i, 0x10, 0);
    run_case(3, 0, 0x10, 0);
    printf("halo owner draw: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
