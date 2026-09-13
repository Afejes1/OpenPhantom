#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
static int checks, failures, calls, expected_3d, expected_index;
static void *expected_handle;
static char handles[8];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void mutate_selected(void)
{
    op_sound_channels[expected_index].is3d = !expected_3d;
    op_sound_channels[expected_index].sample = &handles[6];
    op_sound_channels[expected_index].sample3d = &handles[7];
    op_sound_channels[expected_index].flags ^= 0x80;
}

static void __stdcall release_2d(void *handle)
{
    ++calls;
    check(!expected_3d && handle == expected_handle, "2D branch and handle");
    mutate_selected();
}

static void __stdcall release_3d(void *handle)
{
    ++calls;
    check(expected_3d && handle == expected_handle, "3D branch and handle");
    mutate_selected();
}

op_sound_release_fn op_sound_release_2d = release_2d;
op_sound_release_fn op_sound_release_3d = release_3d;

static void run_case(int index, int is3d, void *handle)
{
    op_sound_channel before[12];
    memset(op_sound_channels, 0x62, sizeof op_sound_channels);
    op_sound_channels[index].is3d = is3d;
    op_sound_channels[index].sample = is3d ? &handles[0] : handle;
    op_sound_channels[index].sample3d = is3d ? handle : &handles[1];
    memcpy(before, op_sound_channels, sizeof before);
    expected_index = index;
    expected_3d = is3d != 0;
    expected_handle = handle;
    calls = 0;
    op_sound_release_handle(index);
    check(calls == 1, "one release callback");
    if (expected_3d) {
        check(op_sound_channels[index].sample3d == 0, "selected 3D handle cleared");
        check(op_sound_channels[index].sample == &handles[6], "callback 2D mutation retained");
    } else {
        check(op_sound_channels[index].sample == 0, "selected 2D handle cleared");
        check(op_sound_channels[index].sample3d == &handles[7], "callback 3D mutation retained");
    }
    check(op_sound_channels[index].is3d == !expected_3d,
          "callback type mutation does not re-gate");
    check(memcmp(op_sound_channels, before, index * sizeof before[0]) == 0 &&
          memcmp(op_sound_channels + index + 1, before + index + 1,
                 (11 - index) * sizeof before[0]) == 0,
          "neighbor records preserved");
}

int main(void)
{
    check(sizeof(op_sound_channel) == 128 &&
          offsetof(op_sound_channel, sample3d) == 4 &&
          offsetof(op_sound_channel, is3d) == 8,
          "channel ABI");
    run_case(0, 0, &handles[2]);
    run_case(11, -9, &handles[3]);
    run_case(5, 0, 0);
    run_case(6, 2, 0);
    printf("Sound release handle: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}