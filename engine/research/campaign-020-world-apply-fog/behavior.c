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
static int checks, failures, events[12], event_count, mode, get_count;
static unsigned int args[12][3], get_values[3];
static void check(int value)
{
    ++checks;
    if (!value)
        ++failures;
}
static void log_event(int id, unsigned int a, unsigned int b, unsigned int c)
{
    if (event_count < 12)
    {
        events[event_count] = id;
        args[event_count][0] = a;
        args[event_count][1] = b;
        args[event_count][2] = c;
    }
    ++event_count;
}
void op_set_fog_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    log_event(1, r, g, b);
    if (mode == 1)
    {
        state.world.color = 0;
        state.world.fog_start = 3.0f;
        state.world.fog_end = 4.0f;
        state.world.flags ^= 1;
    }
}
void op_set_fog_range(float start, float end)
{
    unsigned int a, b;
    memcpy(&a, &start, 4);
    memcpy(&b, &end, 4);
    log_event(2, a, b, 0);
    if (mode == 2)
    {
        state.world.color = 0;
        state.world.fog_start = 3.0f;
        state.world.fog_end = 4.0f;
        state.world.flags ^= 1;
    }
}
unsigned int op_get_render_flags(void)
{
    unsigned int value;
    check(get_count >= 0 && get_count < 3);
    if (get_count < 0 || get_count >= 3)
        return 0;
    value = get_values[get_count++];
    log_event(3, value, 0, 0);
    return value;
}
void op_set_render_flags(unsigned int flags)
{
    log_event(4, flags, 0, 0);
}
void op_set_clear_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    log_event(5, r, g, b);
}
void op_set_clear_pixel(unsigned short pixel)
{
    log_event(6, pixel, 0, 0);
}
static void run_case(unsigned int flags, int mutate, unsigned int color, unsigned int red, unsigned int green,
                     unsigned int blue)
{
    GUARDED expected;
    int i;
    memset(&state, 0x6a, sizeof(state));
    state.before = 0x11223344U;
    state.after = 0x55667788U;
    state.world.flags = flags;
    state.world.color = color;
    state.world.fog_start = 10.0f;
    state.world.fog_end = 22.0f;
    expected = state;
    if (mutate)
    {
        expected.world.color = 0;
        expected.world.fog_start = 3.0f;
        expected.world.fog_end = 4.0f;
        expected.world.flags = flags ^ 1;
    }
    event_count = get_count = 0;
    mode = mutate;
    get_values[0] = 0xabcdef7fU;
    get_values[1] = 0x12345600U;
    op_world_apply_fog(&state.world);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    check(events[0] == 1);
    check(args[0][0] == red && args[0][1] == green && args[0][2] == blue);
    check(events[1] == 2);
    check(args[1][0] == (mutate == 1 ? 0x40400000U : 0x41200000U));
    check(args[1][1] == (mutate == 1 ? 0x40800000U : 0x41b00000U));
    if ((expected.world.flags & 1) != 0)
    {
        check(event_count == 7);
        check(get_count == 2);
        check(events[2] == 3 && events[3] == 4 && events[4] == 5 && events[5] == 3 && events[6] == 4);
        check(args[3][0] == 0xabcdef3fU);
        check(args[4][0] == red && args[4][1] == green && args[4][2] == blue);
        check(args[6][0] == 0x12345640U);
    }
    else
    {
        check(event_count == 5);
        check(get_count == 1);
        check(events[2] == 6 && events[3] == 3 && events[4] == 4);
        check(args[2][0] == 0);
        check(args[4][0] == 0xabcdef3fU);
    }
    for (i = 0; i < event_count && i < 12; ++i)
        check(events[i] != 0);
}
int main(void)
{
    run_case(1, 0, 0xaa80ff01U, 0x80, 0xff, 1);
    run_case(2, 0, 0xaa80ff01U, 0x80, 0xff, 1);
    run_case(0, 1, 0xaa80ff01U, 0x80, 0xff, 1);
    run_case(1, 1, 0xaa80ff01U, 0x80, 0xff, 1);
    run_case(0, 2, 0x00332211U, 0x33, 0x22, 0x11);
    run_case(1, 2, 0x00332211U, 0x33, 0x22, 0x11);
    run_case(1, 0, 0, 0, 0, 0);
    run_case(1, 0, 0xffffffffU, 0xff, 0xff, 0xff);
    run_case(1, 0, 0x80000000U, 0, 0, 0);
    run_case(1, 0, 0x007f80ffU, 0x7f, 0x80, 0xff);
    printf("world fog: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}