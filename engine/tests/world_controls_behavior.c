/* Shared canonical world controls; included once by behavior.c. */
#include "../src/b3d_chunk_readers.h"
#include <float.h>
#include <string.h>

static int effects_state_active;
static unsigned int effects_get_render_flags(void);
static void effects_set_render_flags(unsigned int flags);
static void effects_set_fog_rgb(unsigned int r, unsigned int g, unsigned int b);
static void effects_set_fog_range(float start, float end);
static void effects_set_clear_rgb(unsigned char r, unsigned char g, unsigned char b);
static void effects_set_clear_pixel(unsigned short pixel);

typedef struct world_controls_clock_GUARDED
{
    unsigned int before;
    OP_B3D_WORLD world;
    unsigned int after;
} world_controls_clock_GUARDED;
static int world_controls_clock_checks, world_controls_clock_failures;
static void world_controls_clock_check(int value)
{
    ++world_controls_clock_checks;
    if (!value)
        ++world_controls_clock_failures;
}
static unsigned int world_controls_clock_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static float world_controls_clock_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static void world_controls_clock_run_case(float input, int expected_ticks, unsigned int expected_delta)
{
    world_controls_clock_GUARDED world_controls_clock_state, expected;
    memset(&world_controls_clock_state, 0x5a, sizeof(world_controls_clock_state));
    world_controls_clock_state.before = 0x11223344U;
    world_controls_clock_state.after = 0x55667788U;
    world_controls_clock_state.world.current_ticks = -77;
    world_controls_clock_state.world.previous_ticks = 88;
    world_controls_clock_state.world.seconds = 0.25f;
    world_controls_clock_state.world.delta = 9.0f;
    expected = world_controls_clock_state;
    expected.world.previous_ticks = -77;
    expected.world.current_ticks = expected_ticks;
    expected.world.delta = world_controls_clock_from_bits(expected_delta);
    expected.world.seconds = input;
    op_world_set_clock(&world_controls_clock_state.world, input);
    world_controls_clock_check(memcmp(&world_controls_clock_state, &expected, sizeof(world_controls_clock_state)) == 0);
    world_controls_clock_check(world_controls_clock_bits(world_controls_clock_state.world.seconds) ==
                               world_controls_clock_bits(input));
}
static void world_controls_clock_run_sequence(void)
{
    world_controls_clock_GUARDED world_controls_clock_state, expected;
    memset(&world_controls_clock_state, 0x62, sizeof(world_controls_clock_state));
    world_controls_clock_state.before = 0x11223344U;
    world_controls_clock_state.after = 0x55667788U;
    world_controls_clock_state.world.current_ticks = 17;
    world_controls_clock_state.world.seconds = 1.0f;
    world_controls_clock_state.world.previous_ticks = 22;
    world_controls_clock_state.world.delta = 7.0f;
    expected = world_controls_clock_state;
    expected.world.current_ticks = 1125;
    expected.world.previous_ticks = 17;
    expected.world.seconds = 1.125f;
    expected.world.delta = 0.125f;
    op_world_set_clock(&world_controls_clock_state.world, 1.125f);
    world_controls_clock_check(memcmp(&world_controls_clock_state, &expected, sizeof(world_controls_clock_state)) == 0);
    expected.world.current_ticks = -125;
    expected.world.previous_ticks = 1125;
    expected.world.seconds = -0.125f;
    expected.world.delta = -1.25f;
    op_world_set_clock(&world_controls_clock_state.world, -0.125f);
    world_controls_clock_check(memcmp(&world_controls_clock_state, &expected, sizeof(world_controls_clock_state)) == 0);
}

static int world_controls_clock_main(void)
{
#ifdef OP_VC5_BEHAVIOR
    unsigned int before = _controlfp(0, 0);
    _clearfp();
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
#endif
    op_world_set_clock(0, 1.0f);
    world_controls_clock_run_case(0.0f, 0, 0xbe800000U);
    world_controls_clock_run_case(world_controls_clock_from_bits(0x80000000U), 0, 0xbe800000U);
    world_controls_clock_run_case(1.0f, 1000, 0x3f400000U);
    world_controls_clock_run_case(-1.5f, -1500, 0xbfe00000U);
    world_controls_clock_run_case(0.5f, 500, 0x3e800000U);
#ifdef OP_VC5_BEHAVIOR
    world_controls_clock_run_case(0.01f, 9, 0xbe75c28fU);
    world_controls_clock_run_case(-0.01f, -9, 0xbe851eb8U);
#endif
    world_controls_clock_run_sequence();
#ifdef OP_VC5_BEHAVIOR
    _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
#endif
    printf("world clock: %d checks, %d failures\n", world_controls_clock_checks, world_controls_clock_failures);
    return world_controls_clock_failures != 0;
}
typedef struct world_controls_ambient_GUARDED
{
    unsigned int before;
    OP_B3D_WORLD world;
    unsigned int after;
} world_controls_ambient_GUARDED;
static int world_controls_ambient_checks, world_controls_ambient_failures;
static void world_controls_ambient_check(int value)
{
    ++world_controls_ambient_checks;
    if (!value)
        ++world_controls_ambient_failures;
}
static float world_controls_ambient_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int world_controls_ambient_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static void world_controls_ambient_run_case(unsigned int input_bits, unsigned int expected_bits)
{
    world_controls_ambient_GUARDED world_controls_ambient_state, expected;
    float input = world_controls_ambient_from_bits(input_bits);
    memset(&world_controls_ambient_state, 0x69, sizeof(world_controls_ambient_state));
    world_controls_ambient_state.before = 0x11223344U;
    world_controls_ambient_state.after = 0x55667788U;
    expected = world_controls_ambient_state;
    memcpy(&expected.world.ambient, &expected_bits, 4);
    op_world_set_ambient(&world_controls_ambient_state.world, input);
    world_controls_ambient_check(
        memcmp(&world_controls_ambient_state, &expected, sizeof(world_controls_ambient_state)) == 0);
    world_controls_ambient_check(world_controls_ambient_bits(world_controls_ambient_state.world.ambient) ==
                                 expected_bits);
}
static int world_controls_ambient_main(void)
{
#ifdef OP_VC5_BEHAVIOR
    unsigned int before = _controlfp(0, 0);
    _clearfp();
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
#endif
    world_controls_ambient_run_case(0xbf800000U, 0);
    world_controls_ambient_run_case(0x80000000U, 0x80000000U);
    world_controls_ambient_run_case(0, 0);
    world_controls_ambient_run_case(0x3e800000U, 0x3e800000U);
    world_controls_ambient_run_case(0x3f800000U, 0x3f800000U);
    world_controls_ambient_run_case(0x40000000U, 0x3f800000U);
#ifdef OP_VC5_BEHAVIOR
    world_controls_ambient_run_case(0x7fc00000U, 0);
    world_controls_ambient_run_case(0x7f800000U, 0x3f800000U);
    world_controls_ambient_run_case(0xff800000U, 0);
    world_controls_ambient_run_case(0xffc12345U, 0);
#endif
    world_controls_ambient_run_case(0x00000001U, 0x00000001U);
    world_controls_ambient_run_case(0x80000001U, 0);
    world_controls_ambient_run_case(0x3f7fffffU, 0x3f7fffffU);
    world_controls_ambient_run_case(0x3f800001U, 0x3f800000U);
#ifdef OP_VC5_BEHAVIOR
    _clearfp();
    _controlfp(before, _MCW_EM | _MCW_PC | _MCW_RC);
#endif
    printf("world ambient: %d checks, %d failures\n", world_controls_ambient_checks, world_controls_ambient_failures);
    return world_controls_ambient_failures != 0;
}
typedef struct world_controls_fog_GUARDED
{
    unsigned int before;
    OP_B3D_WORLD world;
    unsigned int after;
} world_controls_fog_GUARDED;
static world_controls_fog_GUARDED world_controls_fog_state;
static int world_controls_fog_checks, world_controls_fog_failures, world_controls_fog_events[12],
    world_controls_fog_event_count, world_controls_fog_mode, world_controls_fog_get_count;
static unsigned int world_controls_fog_args[12][3], world_controls_fog_get_values[3];
static void world_controls_fog_check(int value)
{
    ++world_controls_fog_checks;
    if (!value)
        ++world_controls_fog_failures;
}
static int world_controls_sequence_active;
static world_controls_fog_GUARDED world_controls_sequence_expected;
static void world_controls_sequence_observe(void)
{
    if (world_controls_sequence_active)
        world_controls_fog_check(memcmp(&world_controls_fog_state, &world_controls_sequence_expected,
                                        sizeof(world_controls_fog_state)) == 0);
}
static void world_controls_fog_log_event(int id, unsigned int a, unsigned int b, unsigned int c)
{
    if (world_controls_fog_event_count < 12)
    {
        world_controls_fog_events[world_controls_fog_event_count] = id;
        world_controls_fog_args[world_controls_fog_event_count][0] = a;
        world_controls_fog_args[world_controls_fog_event_count][1] = b;
        world_controls_fog_args[world_controls_fog_event_count][2] = c;
    }
    ++world_controls_fog_event_count;
}
void op_set_fog_rgb(unsigned int r, unsigned int g, unsigned int b)
{
    if (effects_state_active)
    {
        effects_set_fog_rgb(r, g, b);
        return;
    }
    world_controls_sequence_observe();
    world_controls_fog_log_event(1, r, g, b);
    if (world_controls_fog_mode == 1)
    {
        world_controls_fog_state.world.color = 0;
        world_controls_fog_state.world.fog_start = 3.0f;
        world_controls_fog_state.world.fog_end = 4.0f;
        world_controls_fog_state.world.flags ^= 1;
    }
}
void op_set_fog_range(float start, float end)
{
    unsigned int a, b;
    if (effects_state_active)
    {
        effects_set_fog_range(start, end);
        return;
    }
    world_controls_sequence_observe();
    memcpy(&a, &start, 4);
    memcpy(&b, &end, 4);
    world_controls_fog_log_event(2, a, b, 0);
    if (world_controls_fog_mode == 2)
    {
        world_controls_fog_state.world.color = 0;
        world_controls_fog_state.world.fog_start = 3.0f;
        world_controls_fog_state.world.fog_end = 4.0f;
        world_controls_fog_state.world.flags ^= 1;
    }
}
unsigned int op_get_render_flags(void)
{
    unsigned int value;
    if (effects_state_active)
        return effects_get_render_flags();
    world_controls_sequence_observe();
    world_controls_fog_check(world_controls_fog_get_count >= 0 && world_controls_fog_get_count < 3);
    if (world_controls_fog_get_count < 0 || world_controls_fog_get_count >= 3)
        return 0;
    value = world_controls_fog_get_values[world_controls_fog_get_count++];
    world_controls_fog_log_event(3, value, 0, 0);
    return value;
}
void op_set_render_flags(unsigned int flags)
{
    if (effects_state_active)
    {
        effects_set_render_flags(flags);
        return;
    }
    world_controls_sequence_observe();
    world_controls_fog_log_event(4, flags, 0, 0);
}
void op_set_clear_rgb(unsigned char r, unsigned char g, unsigned char b)
{
    if (effects_state_active)
    {
        effects_set_clear_rgb(r, g, b);
        return;
    }
    world_controls_sequence_observe();
    world_controls_fog_log_event(5, r, g, b);
}
void op_set_clear_pixel(unsigned short pixel)
{
    if (effects_state_active)
    {
        effects_set_clear_pixel(pixel);
        return;
    }
    world_controls_sequence_observe();
    world_controls_fog_log_event(6, pixel, 0, 0);
}
static void world_controls_fog_run_case(unsigned int flags, int mutate, unsigned int color, unsigned int red,
                                        unsigned int green, unsigned int blue)
{
    world_controls_fog_GUARDED expected;
    int i;
    memset(&world_controls_fog_state, 0x6a, sizeof(world_controls_fog_state));
    world_controls_fog_state.before = 0x11223344U;
    world_controls_fog_state.after = 0x55667788U;
    world_controls_fog_state.world.flags = flags;
    world_controls_fog_state.world.color = color;
    world_controls_fog_state.world.fog_start = 10.0f;
    world_controls_fog_state.world.fog_end = 22.0f;
    expected = world_controls_fog_state;
    if (mutate)
    {
        expected.world.color = 0;
        expected.world.fog_start = 3.0f;
        expected.world.fog_end = 4.0f;
        expected.world.flags = flags ^ 1;
    }
    world_controls_fog_event_count = world_controls_fog_get_count = 0;
    world_controls_fog_mode = mutate;
    world_controls_fog_get_values[0] = 0xabcdef7fU;
    world_controls_fog_get_values[1] = 0x12345600U;
    op_world_apply_fog(&world_controls_fog_state.world);
    world_controls_fog_check(memcmp(&world_controls_fog_state, &expected, sizeof(world_controls_fog_state)) == 0);
    world_controls_fog_check(world_controls_fog_events[0] == 1);
    world_controls_fog_check(world_controls_fog_args[0][0] == red && world_controls_fog_args[0][1] == green &&
                             world_controls_fog_args[0][2] == blue);
    world_controls_fog_check(world_controls_fog_events[1] == 2);
    world_controls_fog_check(world_controls_fog_args[1][0] == (mutate == 1 ? 0x40400000U : 0x41200000U));
    world_controls_fog_check(world_controls_fog_args[1][1] == (mutate == 1 ? 0x40800000U : 0x41b00000U));
    if ((expected.world.flags & 1) != 0)
    {
        world_controls_fog_check(world_controls_fog_event_count == 7);
        world_controls_fog_check(world_controls_fog_get_count == 2);
        world_controls_fog_check(world_controls_fog_events[2] == 3 && world_controls_fog_events[3] == 4 &&
                                 world_controls_fog_events[4] == 5 && world_controls_fog_events[5] == 3 &&
                                 world_controls_fog_events[6] == 4);
        world_controls_fog_check(world_controls_fog_args[3][0] == 0xabcdef3fU);
        world_controls_fog_check(world_controls_fog_args[4][0] == red && world_controls_fog_args[4][1] == green &&
                                 world_controls_fog_args[4][2] == blue);
        world_controls_fog_check(world_controls_fog_args[6][0] == 0x12345640U);
    }
    else
    {
        world_controls_fog_check(world_controls_fog_event_count == 5);
        world_controls_fog_check(world_controls_fog_get_count == 1);
        world_controls_fog_check(world_controls_fog_events[2] == 6 && world_controls_fog_events[3] == 3 &&
                                 world_controls_fog_events[4] == 4);
        world_controls_fog_check(world_controls_fog_args[2][0] == 0);
        world_controls_fog_check(world_controls_fog_args[4][0] == 0xabcdef3fU);
    }
    for (i = 0; i < world_controls_fog_event_count && i < 12; ++i)
        world_controls_fog_check(world_controls_fog_events[i] != 0);
}
static int world_controls_fog_main(void)
{
    world_controls_fog_run_case(1, 0, 0xaa80ff01U, 0x80, 0xff, 1);
    world_controls_fog_run_case(2, 0, 0xaa80ff01U, 0x80, 0xff, 1);
    world_controls_fog_run_case(0, 1, 0xaa80ff01U, 0x80, 0xff, 1);
    world_controls_fog_run_case(1, 1, 0xaa80ff01U, 0x80, 0xff, 1);
    world_controls_fog_run_case(0, 2, 0x00332211U, 0x33, 0x22, 0x11);
    world_controls_fog_run_case(1, 2, 0x00332211U, 0x33, 0x22, 0x11);
    world_controls_fog_run_case(1, 0, 0, 0, 0, 0);
    world_controls_fog_run_case(1, 0, 0xffffffffU, 0xff, 0xff, 0xff);
    world_controls_fog_run_case(1, 0, 0x80000000U, 0, 0, 0);
    world_controls_fog_run_case(1, 0, 0x007f80ffU, 0x7f, 0x80, 0xff);
    printf("world fog: %d checks, %d failures\n", world_controls_fog_checks, world_controls_fog_failures);
    return world_controls_fog_failures != 0;
}
static void world_controls_shared_sequence(void)
{
    world_controls_fog_GUARDED expected;
    memset(&world_controls_fog_state, 0x4b, sizeof(world_controls_fog_state));
    world_controls_fog_state.before = 0x11223344U;
    world_controls_fog_state.after = 0x55667788U;
    world_controls_fog_state.world.current_ticks = 250;
    world_controls_fog_state.world.seconds = 0.25f;
    world_controls_fog_state.world.flags = 1;
    world_controls_fog_state.world.color = 0x123456U;
    world_controls_fog_state.world.fog_start = 10.0f;
    world_controls_fog_state.world.fog_end = 22.0f;
    expected = world_controls_fog_state;
    expected.world.previous_ticks = 250;
    expected.world.current_ticks = 1500;
    expected.world.seconds = 1.5f;
    expected.world.delta = 1.25f;
    op_world_set_clock(&world_controls_fog_state.world, 1.5f);
    world_controls_fog_check(memcmp(&world_controls_fog_state, &expected, sizeof(expected)) == 0);
    expected.world.ambient = 1.0f;
    op_world_set_ambient(&world_controls_fog_state.world, 2.0f);
    world_controls_fog_check(memcmp(&world_controls_fog_state, &expected, sizeof(expected)) == 0);
    world_controls_sequence_expected = expected;
    world_controls_sequence_active = 1;
    world_controls_fog_mode = 0;
    world_controls_fog_event_count = world_controls_fog_get_count = 0;
    world_controls_fog_get_values[0] = 0x876543ffU;
    world_controls_fog_get_values[1] = 0x12345600U;
    op_world_apply_fog(&world_controls_fog_state.world);
    world_controls_sequence_active = 0;
    world_controls_fog_check(memcmp(&world_controls_fog_state, &expected, sizeof(expected)) == 0);
    world_controls_fog_check(world_controls_fog_event_count == 7 && world_controls_fog_get_count == 2);
    world_controls_fog_check(world_controls_fog_events[0] == 1 && world_controls_fog_events[1] == 2 &&
                             world_controls_fog_events[2] == 3 && world_controls_fog_events[3] == 4 &&
                             world_controls_fog_events[4] == 5 && world_controls_fog_events[5] == 3 &&
                             world_controls_fog_events[6] == 4);
    world_controls_fog_check(world_controls_fog_args[0][0] == 0x12 && world_controls_fog_args[0][1] == 0x34 &&
                             world_controls_fog_args[0][2] == 0x56);
    world_controls_fog_check(world_controls_fog_args[1][0] == 0x41200000U &&
                             world_controls_fog_args[1][1] == 0x41b00000U);
    world_controls_fog_check(world_controls_fog_args[3][0] == 0x876543bfU &&
                             world_controls_fog_args[6][0] == 0x12345640U);
    world_controls_fog_check(world_controls_fog_args[4][0] == 0x12 && world_controls_fog_args[4][1] == 0x34 &&
                             world_controls_fog_args[4][2] == 0x56);
}
static int op_test_world_controls(void)
{
    world_controls_clock_main();
    world_controls_ambient_main();
    world_controls_fog_main();
    world_controls_shared_sequence();
    printf("world controls integrated: %d checks, %d failures\n",
           world_controls_clock_checks + world_controls_ambient_checks + world_controls_fog_checks,
           world_controls_clock_failures + world_controls_ambient_failures + world_controls_fog_failures);
    return world_controls_clock_failures + world_controls_ambient_failures + world_controls_fog_failures;
}
