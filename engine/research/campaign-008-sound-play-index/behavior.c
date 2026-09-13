#include "api.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>

op_sound_world_state *op_sound_world;
int op_sound_mode;
static op_sound_world_state world;
static op_sound_descriptor descriptors[3];
static int checks, failures, calls, result_value;
static int expected_mode;
static op_sound_descriptor *expected_descriptor;
static int *expected_handle;
static float *expected_position;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_sound_start_descriptor(op_sound_descriptor *descriptor, int *handle,
                              float *position)
{
    ++calls;
    check(op_sound_mode == expected_mode, "negative mode defaults to eight before callback");
    check(descriptor == expected_descriptor, "descriptor stride pointer");
    check(handle == expected_handle && position == expected_position,
          "output pointer identities forwarded");
    op_sound_mode = 91;
    world.descriptor_count = 0;
    return result_value;
}

static void accepted(unsigned int index, int mode, int callback_mode,
                     int *handle, float *position)
{
    if (index >= 3) {
        check(0, "owned descriptor index is bounded");
        return;
    }
    calls = 0;
    world.descriptor_count = 3;
    world.descriptors = descriptors;
    op_sound_world = &world;
    op_sound_mode = mode;
    expected_mode = callback_mode;
    expected_descriptor = &descriptors[index];
    expected_handle = handle;
    expected_position = position;
    result_value = -27;
    op_sound_play_index(index, handle, position);
    check(calls == 1, "accepted index calls start once");
    check(op_sound_mode == 91 && world.descriptor_count == 0,
          "callback mutations retained");
}

static void rejected(unsigned int index)
{
    calls = 0;
    world.descriptor_count = 3;
    world.descriptors = descriptors;
    op_sound_world = &world;
    op_sound_mode = -4;
    op_sound_play_index(index, 0, 0);
    check(calls == 0 && op_sound_mode == -4,
          "rejected unsigned index returns untouched");
}

int main(void)
{
    int handle;
    float position[3];
    check(sizeof(op_sound_descriptor) == 64 &&
          offsetof(op_sound_world_state, descriptor_count) == 0xcc4 &&
          offsetof(op_sound_world_state, descriptors) == 0xcc8,
          "descriptor and world ABI");
    accepted(0, -1, 8, &handle, position);
    accepted(2, 0, 0, 0, position);
    accepted(1, 8, 8, &handle, 0);
    accepted(1, INT_MIN, 8, &handle, position);
    accepted(0, 7, 7, &handle, position);
    rejected(3);
    rejected(UINT_MAX);
    printf("Sound play index: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
