#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
static int checks, failures, calls, expected_type;
static unsigned int expected_index;
static void *initial_handle, *changed_handle;
static char handles[8];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", description); }
}

static void loop_call(int type, void *sample, int loops)
{
    ++calls;
    check(expected_index < 12, "loop callback index bounded");
    if (expected_index >= 12) return;
    check(type == expected_type && sample == initial_handle && loops == 0,
          "loop branch, handle and count");
    op_sound_channels[expected_index].type = !expected_type;
    if (expected_type)
        op_sound_channels[expected_index].sample3d = changed_handle;
    else
        op_sound_channels[expected_index].sample = changed_handle;
    op_sound_channels[expected_index].flags = 0x40000021U;
}

static void start_call(int type, void *sample)
{
    ++calls;
    check(expected_index < 12, "start callback index bounded");
    if (expected_index >= 12) return;
    check(type == expected_type && sample == changed_handle,
          "start stays on chosen branch and reloads handle");
    op_sound_channels[expected_index].flags = 0x80000041U;
}

static void __stdcall loop2d(void *sample, int loops) { loop_call(0, sample, loops); }
static void __stdcall loop3d(void *sample, int loops) { loop_call(1, sample, loops); }
static void __stdcall start2d(void *sample) { start_call(0, sample); }
static void __stdcall start3d(void *sample) { start_call(1, sample); }
op_sound_loop_fn op_sound_loop2d = loop2d;
op_sound_loop_fn op_sound_loop3d = loop3d;
op_sound_start_fn op_sound_start2d = start2d;
op_sound_start_fn op_sound_start3d = start3d;

static void run_case(unsigned int index, int type, unsigned int flags, int looped)
{
    op_sound_channel before[12];
    op_sound_channel *channel;
    if (index >= 12) { check(0, "case index bounded"); return; }
    memset(op_sound_channels, 0x62, sizeof op_sound_channels);
    channel = &op_sound_channels[index];
    initial_handle = &handles[0];
    changed_handle = &handles[4];
    channel->sample = initial_handle;
    channel->sample3d = initial_handle;
    channel->type = type;
    channel->flags = flags;
    memcpy(before, op_sound_channels, sizeof before);
    expected_index = index;
    expected_type = type != 0;
    calls = 0;
    if (!looped)
        changed_handle = initial_handle;
    op_sound_start_sample(index);
    check(calls == (looped ? 2 : 1), "expected callback count");
    check(channel->flags == 0x80020041U,
          "live start mutation preserved with active bit");
    check(memcmp(op_sound_channels, before, index * sizeof before[0]) == 0 &&
          memcmp(op_sound_channels + index + 1, before + index + 1,
                 (11 - index) * sizeof before[0]) == 0,
          "neighbor channels unchanged");
}

int main(void)
{
    op_sound_channel before[12];
    check(sizeof(op_sound_channel) == 128 && offsetof(op_sound_channel, flags) == 16,
          "channel ABI");
    run_case(0, 0, 0x10U, 1);
    run_case(11, -3, 0x10U, 1);
    run_case(5, 0, 0U, 0);
    run_case(6, 7, 0U, 0);
    memset(op_sound_channels, 0x44, sizeof op_sound_channels);
    memcpy(before, op_sound_channels, sizeof before);
    calls = 0;
    op_sound_start_sample(12);
    op_sound_start_sample(~0U);
    check(calls == 0 && memcmp(before, op_sound_channels, sizeof before) == 0,
          "out-of-range indices return untouched");
    printf("Sound start sample: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
