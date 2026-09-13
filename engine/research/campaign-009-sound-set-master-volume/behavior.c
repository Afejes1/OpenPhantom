#include "api.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized;
float op_sound_master_gain;
void *op_sound_driver;
op_sound_channel op_sound_channels[12];
static int checks, failures, call_count, channel_call_count;
static int expected_values[12], expected_count, expected_master_value;
static unsigned long expected_gain_bits;
static int mutation_case;
static char handles[12], drivers[2];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", description); }
}

static unsigned long bits(float value)
{
    unsigned long result;
    memcpy(&result, &value, 4);
    return result;
}

static float from_bits(unsigned long value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}

static void __stdcall set_volume(void *handle, int volume)
{
    int index = channel_call_count;
    ++call_count;
    if (index < 0 || index >= expected_count || index >= 12) {
        check(0, "volume callback index bounded");
        return;
    }
    check(handle == &handles[index] && volume == expected_values[index],
          "per-channel handle and independent integer oracle");
    ++channel_call_count;
    if (mutation_case && index == 0) {
        op_sound_master_gain = 0.5f;
        op_sound_channels[1].type = 1;
        op_sound_driver = &drivers[1];
        op_sound_initialized = 0;
    }
}

static void __stdcall set_master(void *driver, int value)
{
    ++call_count;
    check(channel_call_count == expected_count, "master call follows channel calls");
    check(driver == (mutation_case ? &drivers[1] : &drivers[0]) &&
          value == expected_master_value, "live driver and original value forwarded");
    check(bits(op_sound_master_gain) ==
          (mutation_case ? 0x3f000000UL : expected_gain_bits),
          "master callback observes stored or callback-mutated gain bits");
}

op_sound_volume_fn op_sound_set_volume3d = set_volume;
op_sound_volume_fn op_sound_set_master = set_master;

static void master_only(int value, unsigned long gain_bits)
{
    op_sound_channel before[12];
    memset(op_sound_channels, 0x35, sizeof op_sound_channels);
    op_sound_channels[0].type = 0;
    op_sound_channels[1].type = 2;
    op_sound_channels[2].type = -1;
    memcpy(before, op_sound_channels, sizeof before);
    op_sound_initialized = 1;
    op_sound_driver = &drivers[0];
    expected_master_value = value;
    expected_gain_bits = gain_bits;
    expected_count = channel_call_count = call_count = 0;
    mutation_case = 0;
    op_sound_set_master_volume(value);
    check(call_count == 1, "non-type-one records produce only master callback");
    check(bits(op_sound_master_gain) == gain_bits, "independent master gain bits");
    check(memcmp(before, op_sound_channels, sizeof before) == 0,
          "skipped channels remain byte-identical");
}

static void numeric_case(void)
{
    static const unsigned long volume_bits[6] = {
        0x3f000000UL, 0x3e800000UL, 0x3f800000UL,
        0x40000000UL, 0xbf000000UL, 0x7fc12345UL
    };
    static const int results[6] = {63, 31, 127, 127, 0, 0};
    op_sound_channel before[12];
    int i;
    memset(op_sound_channels, 0, sizeof op_sound_channels);
    for (i = 0; i < 6; ++i) {
        op_sound_channels[i].type = 1;
        op_sound_channels[i].sample3d = &handles[i];
        op_sound_channels[i].volume = from_bits(volume_bits[i]);
        expected_values[i] = results[i];
    }
    memcpy(before, op_sound_channels, sizeof before);
    op_sound_initialized = 1;
    op_sound_driver = &drivers[0];
    expected_master_value = 127;
    expected_gain_bits = 0x3f800000UL;
    expected_count = 6;
    channel_call_count = call_count = 0;
    mutation_case = 0;
    op_sound_set_master_volume(127);
    check(call_count == 7 && channel_call_count == 6,
          "six ordered channel callbacks then master");
    check(memcmp(before, op_sound_channels, sizeof before) == 0,
          "numeric pass does not mutate channel records");
}

static void live_mutation_case(void)
{
    memset(op_sound_channels, 0, sizeof op_sound_channels);
    op_sound_channels[0].type = 1;
    op_sound_channels[0].sample3d = &handles[0];
    op_sound_channels[0].volume = 0.5f;
    op_sound_channels[1].type = 0;
    op_sound_channels[1].sample3d = &handles[1];
    op_sound_channels[1].volume = 1.0f;
    expected_values[0] = 63;
    expected_values[1] = 63;
    expected_count = 2;
    channel_call_count = call_count = 0;
    expected_master_value = 127;
    expected_gain_bits = 0x3f800000UL;
    mutation_case = 1;
    op_sound_initialized = 1;
    op_sound_driver = &drivers[0];
    op_sound_set_master_volume(127);
    check(call_count == 3 && channel_call_count == 2,
          "callback changes affect later slot without re-gating");
    check(op_sound_initialized == 0 && op_sound_driver == &drivers[1],
          "callback global mutations retained");
}

int main(void)
{
    check(sizeof(op_sound_channel) == 128 &&
          offsetof(op_sound_channel, volume) == 0x18, "channel ABI");
    op_sound_initialized = 0;
    op_sound_master_gain = 3.0f;
    call_count = 0;
    op_sound_set_master_volume(64);
    check(call_count == 0 && bits(op_sound_master_gain) == 0x40400000UL,
          "uninitialized path untouched");
    master_only(0, 0x00000000UL);
    master_only(64, 0x3f010204UL);
    master_only(127, 0x3f800000UL);
    master_only(-127, 0xbf800000UL);
    master_only(INT_MAX, 0x4b810204UL);
    master_only(INT_MIN, 0xcb810204UL);
    numeric_case();
    live_mutation_case();
    printf("Sound set master volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}