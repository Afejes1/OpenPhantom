#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_channel op_sound_channels[12];
static int checks, failures, calls;
static int expected_slots[12], expected_types[12];
static void *expected_handles[12];
static char handles[24];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void callback(int is3d, void *handle)
{
    int slot;

    if (calls >= 12) {
        check(0, "bounded callback count");
        return;
    }
    slot = expected_slots[calls];
    check(expected_types[calls] == is3d, "callback type and order");
    check(expected_handles[calls] == handle, "exact handle forwarded");
    check((op_sound_channels[slot].flags & 0x40000) == 0,
          "resume flag cleared before callback");
    ++calls;
    if (slot == 0)
        op_sound_channels[11].flags |= 0x40000;
    op_sound_channels[slot].flags |= 0x80;
}

static void __stdcall resume_2d(void *handle) { callback(0, handle); }
static void __stdcall resume_3d(void *handle) { callback(1, handle); }
op_sound_resume_fn op_sound_resume_2d = resume_2d;
op_sound_resume_fn op_sound_resume_3d = resume_3d;

int main(void)
{
    memset(op_sound_channels, 0, sizeof op_sound_channels);
    check(sizeof(op_sound_channel) == 128 &&
          offsetof(op_sound_channel, flags) == 0x10,
          "channel ABI and stride");
    op_sound_channels[0].flags = 0x40001;
    op_sound_channels[0].sample = &handles[0];
    op_sound_channels[5].flags = 0x40002;
    op_sound_channels[5].is3d = -7;
    op_sound_channels[5].sample3d = 0;
    op_sound_channels[11].sample = &handles[11];
    expected_slots[0] = 0;
    expected_types[0] = 0;
    expected_handles[0] = &handles[0];
    expected_slots[1] = 5;
    expected_types[1] = 1;
    expected_handles[1] = 0;
    expected_slots[2] = 11;
    expected_types[2] = 0;
    expected_handles[2] = &handles[11];
    calls = 0;
    op_sound_resume_all();
    check(calls == 3, "mixed and callback-enabled later slots processed");
    check(op_sound_channels[0].flags == 0x81,
          "other and callback bits preserved after clearing resume bit");
    calls = 0;
    op_sound_resume_all();
    check(calls == 0, "second call skips when callback did not restore bits");
    printf("Sound resume all: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}