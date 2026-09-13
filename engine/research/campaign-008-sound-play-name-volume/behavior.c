#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized;
int op_sound_mode;
static int checks, failures, event_count, events[3], callback_result;
static unsigned long input_bits;
static unsigned int expected_flags;
static const char *expected_name;

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

static float from_bits(unsigned long value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}

static void record(int value)
{
    if (event_count >= 3) {
        check(0, "bounded event capture");
        return;
    }
    events[event_count++] = value;
}

void op_sound_set_field(int field, float value)
{
    record(event_count == 0 ? 1 : 3);
    check(field == 0, "setter field is zero");
    if (event_count == 1) {
        check(bits(value) == input_bits, "first setter receives input bits");
        check(op_sound_mode == 41, "first setter sees old mode");
        op_sound_mode = -5;
        op_sound_initialized = 0;
    } else {
        check(bits(value) == 0x40000000UL, "final setter receives two");
        check(op_sound_mode == 73, "final setter sees callback mode mutation");
        op_sound_initialized = 0;
    }
}

int op_sound_play_descriptor_name(int slot, const char *name, int *handle,
                                  float *position, unsigned int flags)
{
    record(2);
    check(slot == -1 && handle == 0 && position == 0,
          "fixed play arguments");
    check(name == expected_name && flags == expected_flags,
          "name identity and ORed flags forwarded");
    check(op_sound_mode == 2, "play sees mode two");
    op_sound_mode = 73;
    return callback_result;
}

static void active(unsigned long volume_bits, unsigned int flags, int result)
{
    static const char name[] = "owned";
    int returned;
    op_sound_initialized = -1;
    op_sound_mode = 41;
    event_count = 0;
    input_bits = volume_bits;
    expected_flags = flags | 0x800U;
    expected_name = name;
    callback_result = result;
    returned = op_sound_play_name_volume(name, from_bits(volume_bits), flags);
    check(returned == result, "saved play result returned after final setter");
    check(event_count == 3 && events[0] == 1 && events[1] == 2 && events[2] == 3,
          "setter play setter order");
    check(op_sound_initialized == 0 && op_sound_mode == 73,
          "callback mutations retained without initialized re-gate");
}

int main(void)
{
    op_sound_initialized = 0;
    op_sound_mode = 19;
    event_count = 0;
    check(op_sound_play_name_volume(0, from_bits(0x7fc54321UL), ~0U) == -1,
          "uninitialized returns minus one");
    check(event_count == 0 && op_sound_mode == 19,
          "uninitialized path has no writes or callbacks");
    active(0x80000000UL, 0U, 0);
    active(0x7fc54321UL, 1U, INT_MIN);
    active(0x3f000000UL, 0xffffffffU, INT_MAX);
    printf("Sound play name volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
