#include "api.h"
#include <stdio.h>
#include <string.h>

int op_sound_mode;
static int checks, failures, calls, lock_result, expected_release;
static int expected_handle_value;
static int fields[3], field_count;
static unsigned long values[3];
static const char *expected_name;
static int *expected_handle;
static float *expected_position;
static unsigned int expected_flags;

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

void op_sound_release_slot(unsigned int index)
{
    ++calls;
    check(index == (unsigned int)expected_release, "nonnegative handle released first");
    check(op_sound_mode == -4 && field_count == 0,
          "release precedes mode and field setup");
    if (expected_handle != 0)
        *expected_handle = -7;
}

void op_sound_set_field(int field, float value)
{
    if (field_count >= 3) { check(0, "field capture bounded"); return; }
    check(op_sound_mode == 3 + field_count * 10,
          "setter observes expected prior mode mutation");
    fields[field_count] = field;
    values[field_count++] = bits(value);
    op_sound_mode += 10;
}

int op_sound_has_input_lock(int minimum)
{
    check(minimum == 5 && field_count == 2 && op_sound_mode == 23,
          "lock query after first two fields and mutations");
    return lock_result;
}

int op_sound_play_descriptor_name(int slot, const char *name, int *handle,
                                  float *position, unsigned int flags)
{
    ++calls;
    check(slot == 0 && name == expected_name && handle == expected_handle &&
          position == expected_position && flags == expected_flags,
          "final play arguments and identities");
    check(op_sound_mode == (position != 0 ? 33 : 4),
          "final play observes accumulated mode state");
    if (handle != 0)
        check(*handle == expected_handle_value,
              "final play observes release mutation or unchanged negative handle");
    op_sound_mode = 99;
    if (handle != 0)
        *handle = 6;
    return -123;
}

static void run_case(int initial_handle, int use_handle, int use_position,
                     int lock)
{
    static const char name[] = "voice";
    int handle = initial_handle;
    float position[3];
    calls = field_count = 0;
    lock_result = lock;
    expected_name = name;
    expected_handle = use_handle ? &handle : 0;
    expected_position = use_position ? position : 0;
    expected_release = initial_handle;
    expected_handle_value = use_handle && initial_handle >= 0 ? -7 : initial_handle;
    expected_flags = use_position ? 0x4a04U : 0x4201U;
    op_sound_mode = -4;
    op_sound_play_voice(name, expected_handle, expected_position);
    check(calls == ((use_handle && initial_handle >= 0) ? 2 : 1),
          "release and play callback count");
    check(field_count == (use_position ? 3 : 0), "position controls field setters");
    if (use_position) {
        check(fields[0] == 2 && values[0] == 0x42c80000UL &&
              fields[1] == 4 && values[1] == 0x42c80000UL &&
              fields[2] == 3 && values[2] == (lock ? 0x41000000UL : 0x40800000UL),
              "field order and independent float bits");
    }
    check(op_sound_mode == 99, "final callback mutation retained");
    if (use_handle)
        check(handle == 6, "final handle mutation retained");
}

int main(void)
{
    run_case(-1, 1, 1, 0);
    run_case(0, 1, 1, -3);
    run_case(11, 1, 0, 0);
    run_case(12, 1, 0, 0);
    run_case(0, 0, 1, 1);
    run_case(0, 0, 0, 0);
    printf("Sound play voice: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
