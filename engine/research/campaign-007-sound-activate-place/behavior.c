#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized;
op_sound_world_state *op_sound_world;
static op_sound_world_state world;
static op_sound_world_state other_world;
static int checks, failures, random_calls, random_result;
static op_sound_placement *current;
static float callback_span;
static float callback_deadline;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static unsigned long bits(float value)
{
    unsigned long result;
    memcpy(&result, &value, 4);
    return result;
}

static float make_float(unsigned long value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}

int op_sound_random(void)
{
    ++random_calls;
    check(current->active == 1, "active set before random callback");
    check(bits(current->deadline) == bits(world.time),
          "initial world time copied before callback");
    current->random_span = callback_span;
    current->deadline = callback_deadline;
    op_sound_world = &other_world;
    return random_result;
}

static void gated(int initialized, op_sound_placement *placement,
                  op_sound_world_state *state)
{
    op_sound_placement before;
    op_sound_initialized = initialized;
    op_sound_world = state;
    random_calls = 0;
    if (placement != 0)
        before = *placement;
    op_sound_activate_place(placement);
    check(random_calls == 0, "gated path skips random callback");
    if (placement != 0)
        check(memcmp(&before, placement, sizeof before) == 0,
              "gated path preserves placement bytes");
}

static void zero_or_nan(unsigned long span_bits, unsigned long time_bits)
{
    op_sound_placement placement;
    memset(&placement, 0x55, sizeof placement);
    placement.active = 0;
    placement.random_span = make_float(span_bits);
    world.time = make_float(time_bits);
    op_sound_initialized = -7;
    op_sound_world = &world;
    random_calls = 0;
    op_sound_activate_place(&placement);
    check(placement.active == 1, "eligible placement activated");
    check(bits(placement.deadline) == time_bits,
          "zero or unordered span copies exact time bits");
    check(random_calls == 0, "zero or quiet NaN span skips random");
}

static void random_case(int value, float span, float changed_deadline,
                        unsigned long expected_bits)
{
    op_sound_placement placement;
    memset(&placement, 0x66, sizeof placement);
    placement.active = 0;
    placement.random_span = 2.0f;
    world.time = 10.0f;
    op_sound_initialized = 1;
    op_sound_world = &world;
    current = &placement;
    random_result = value;
    callback_span = span;
    callback_deadline = changed_deadline;
    random_calls = 0;
    op_sound_activate_place(&placement);
    check(random_calls == 1, "nonzero span calls random once");
    check(bits(placement.deadline) == expected_bits,
          "late span and deadline reload determine result");
    check(op_sound_world == &other_world, "callback world mutation retained");
}

int main(void)
{
    op_sound_placement placement;
    check(sizeof(op_sound_placement) == 60 &&
          offsetof(op_sound_placement, random_span) == 0x20 &&
          offsetof(op_sound_placement, deadline) == 0x28 &&
          offsetof(op_sound_placement, active) == 0x30 &&
          offsetof(op_sound_world_state, time) == 0x54,
          "placement and world ABI");
    memset(&placement, 0x44, sizeof placement);
    placement.active = 0;
    gated(0, &placement, &world);
    gated(1, 0, &world);
    placement.active = 1;
    gated(1, &placement, &world);
    placement.active = 0;
    gated(1, &placement, 0);
    zero_or_nan(0x00000000UL, 0x80000000UL);
    zero_or_nan(0x80000000UL, 0x7fc12345UL);
    zero_or_nan(0x7fc54321UL, 0x3f800000UL);
    /* Independently derived from exact rational arithmetic with the rounded
       float reciprocal: 20, -49151/16384, and 52613349377/536870912.
       These values require only final binary32 rounding in the observed x87
       sequence; the fixture does not repeat the candidate arithmetic. */
    random_case(0, 3.0f, 20.0f, 0x41a00000UL);
    random_case(16384, 4.0f, -5.0f, 0xc03fff00UL);
    random_case(32767, -2.0f, 100.0f, 0x42c40000UL);
    printf("Sound activate place: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}