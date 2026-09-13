#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
typedef struct GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD world;
    unsigned int after;
} GUARDED_WORLD;
static GUARDED_WORLD storage;
static int checks, failures, allocate_calls, timer_calls, timer_value, timer_mode;
static void check(int value)
{
    ++checks;
    if (!value)
        ++failures;
}
void *op_allocate(unsigned int bytes)
{
    ++allocate_calls;
    check(bytes == 0xcdc);
    check(timer_calls == 0);
    return timer_mode < 0 ? 0 : &storage.world;
}
int op_milliseconds(void)
{
    unsigned char zero[0xcdc] = {0};
    ++timer_calls;
    check(allocate_calls == 1);
    check(memcmp(&storage.world, zero, sizeof(zero)) == 0);
    if (timer_mode == 1)
    {
        storage.world.before_clock[3] = 0x5a;
        storage.world.current_ticks = 9;
        storage.world.seconds = 11.0f;
        storage.world.previous_ticks = 12;
        storage.world.delta = 13.0f;
        storage.world.capacity = 14;
        storage.world.capacity_state = 15;
        storage.world.ambient = 16.0f;
        storage.world.fog_flags = 17;
        storage.world.fog_start = 18.0f;
        storage.world.fog_end = 19.0f;
        storage.world.fog_color = 20;
        storage.world.texture_capacity = 21;
    }
    return timer_value;
}
static void run_case(int mode, int ticks)
{
    unsigned int expected[0x337];
    GUARDED_WORLD before_storage;
    OP_WORLD *result;
    memset(&storage, 0x7b, sizeof(storage));
    storage.before = 0x11223344U;
    storage.after = 0x55667788U;
    memset(expected, 0, sizeof(expected));
    allocate_calls = timer_calls = 0;
    timer_mode = mode;
    timer_value = ticks;
    before_storage = storage;
    result = op_world_allocate();
    if (mode < 0)
    {
        check(result == 0);
        check(memcmp(&storage, &before_storage, sizeof(storage)) == 0);
        check(allocate_calls == 1);
        check(timer_calls == 0);
        check(storage.before == 0x11223344U && storage.after == 0x55667788U);
        return;
    }
    if (mode == 1)
        ((unsigned char *)expected)[3] = 0x5a;
    expected[20] = (unsigned int)ticks;
    expected[22] = (unsigned int)ticks;
    expected[30] = 1024;
    expected[31] = 0;
    expected[24] = 0x3dcccccdU;
    expected[134] = 0x41200000U;
    expected[135] = 0x41b00000U;
    expected[133] = 0x00408020U;
    expected[48] = 32;
    check(result == &storage.world);
    check(allocate_calls == 1);
    check(timer_calls == 1);
    check(memcmp(&storage.world, expected, sizeof(expected)) == 0);
    check(storage.before == 0x11223344U && storage.after == 0x55667788U);
}
int main(void)
{
    run_case(-1, 0);
    run_case(0, 0);
    run_case(0, -1234);
    run_case(1, INT_MIN);
    run_case(1, INT_MAX);
    printf("world allocate: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}