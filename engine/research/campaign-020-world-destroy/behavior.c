#include "api.h"
#include <stdio.h>
#include <string.h>
typedef struct GUARDED
{
    unsigned int before;
    OP_WORLD world;
    unsigned int after;
} GUARDED;
static GUARDED state;
static unsigned char actor, retained, replacement;
static int checks, failures, events[12], count, mode;
static void *expected_actor_before_helpers;
static void *expected_retained_release;
static const OP_WORLD *expected_before_final_release;
static void check(int value)
{
    ++checks;
    if (!value)
        ++failures;
}
static void event(int id, void *pointer)
{
    if (count < 12)
        events[count] = id;
    ++count;
    if (mode == 1 && id == 2)
    {
        state.world.loaded = 0;
        state.world.retained = &replacement;
    }
    if (id == 1)
    {
        check(pointer == &actor);
        if (mode == 2)
        {
            state.world.actor = &replacement;
            state.world.actor_count = 0;
            state.world.loaded = 0;
        }
    }
    if (id == 2)
        check(state.world.actor == expected_actor_before_helpers);
    if (id == 6 && mode == 3)
        state.world.retained = 0;
    if (id == 6 && mode == 4)
        state.world.retained = &replacement;
    if (id == 7)
        check(pointer == expected_retained_release);
    if (id == 8)
    {
        check(pointer == &state.world);
        check(memcmp(&state.world, expected_before_final_release, sizeof(state.world)) == 0);
    }
}
void op_release(void *p)
{
    event(p == &state.world ? 8 : (p == &actor ? 1 : 7), p);
}
void op_world_free_materials(OP_WORLD *w)
{
    check(w == &state.world);
    event(2, w);
}
void op_world_free_pointer_table_220(OP_WORLD *w)
{
    check(w == &state.world);
    event(3, w);
}
void op_world_free_pointer_table_620(OP_WORLD *w)
{
    check(w == &state.world);
    event(4, w);
}
void op_world_free_tail_blocks(OP_WORLD *w)
{
    check(w == &state.world);
    event(5, w);
}
void op_world_free_scripts(OP_WORLD *w)
{
    check(w == &state.world);
    event(6, w);
}
static void run_loaded(int actor_count, void *actor_pointer, void *retained_pointer, int mutate)
{
    OP_WORLD expected;
    int i, result, expected_events[8], expected_count;
    memset(&state, 0x6b, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.world.loaded = 1;
    state.world.actor_count = actor_count;
    state.world.actor = actor_pointer;
    state.world.retained = retained_pointer;
    expected = state.world;
    if (actor_count != 0 && actor_pointer != 0)
        expected.actor = 0;
    if (mutate == 1)
    {
        expected.loaded = 0;
        expected.retained = &replacement;
    }
    if (mutate == 2)
    {
        expected.loaded = 0;
        expected.actor_count = 0;
    }
    if (mutate == 3)
        expected.retained = 0;
    if (mutate == 4)
        expected.retained = &replacement;
    expected_actor_before_helpers = expected.actor;
    expected_retained_release = expected.retained;
    expected_before_final_release = &expected;
    expected_count = 0;
    if (actor_count != 0 && actor_pointer != 0)
        expected_events[expected_count++] = 1;
    for (i = 2; i <= 6; ++i)
        expected_events[expected_count++] = i;
    if (expected.retained != 0)
        expected_events[expected_count++] = 7;
    expected_events[expected_count++] = 8;
    count = 0;
    mode = mutate;
    result = op_world_destroy(&state.world);
    check(result == 0);
    check(state.before == 0x11223344U && state.after == 0x55667788U);
    check(memcmp(&state.world, &expected, sizeof(expected)) == 0);
    check(count == expected_count);
    for (i = 0; i < expected_count; ++i)
        check(events[i] == expected_events[i]);
}
int main(void)
{
    GUARDED before;
    int count_bit, actor_bit, retained_bit;
    check(op_world_destroy(0) == 1);
    memset(&state, 0x4c, sizeof(state));
    state.world.loaded = 0;
    before = state;
    count = 0;
    check(op_world_destroy(&state.world) == 0);
    check(count == 0);
    check(memcmp(&before, &state, sizeof(before)) == 0);
    for (count_bit = 0; count_bit < 2; ++count_bit)
        for (actor_bit = 0; actor_bit < 2; ++actor_bit)
            for (retained_bit = 0; retained_bit < 2; ++retained_bit)
                run_loaded(count_bit ? -1 : 0, actor_bit ? &actor : 0, retained_bit ? &retained : 0, 0);
    run_loaded(1, &actor, &retained, 1);
    run_loaded(1, &actor, &retained, 2);
    run_loaded(1, &actor, &retained, 3);
    run_loaded(1, &actor, 0, 4);
    printf("world destroy: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}