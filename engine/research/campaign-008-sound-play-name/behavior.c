#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_sound_initialized;
int op_sound_mode;
static int checks, failures, calls, callback_result;
static const char *expected_name;
static unsigned int expected_flags;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_sound_play_descriptor_name(int slot, const char *name, int *handle,
                                  float *position, unsigned int flags)
{
    ++calls;
    check(slot == -1 && handle == 0 && position == 0,
          "fixed descriptor arguments");
    check(name == expected_name && flags == expected_flags,
          "name identity and flags forwarded");
    check(op_sound_mode == 1, "mode set before callback");
    op_sound_mode = -77;
    op_sound_initialized = 0;
    return callback_result;
}

static void active(int initialized, const char *name, unsigned int flags,
                   int result)
{
    calls = 0;
    op_sound_initialized = initialized;
    op_sound_mode = 93;
    expected_name = name;
    expected_flags = flags;
    callback_result = result;
    check(op_sound_play_name(name, flags) == result,
          "signed callback result returned unchanged");
    check(calls == 1, "one play callback");
    check(op_sound_mode == -77 && op_sound_initialized == 0,
          "callback global mutations retained");
}

int main(void)
{
    static const char name[] = "owned";
    op_sound_initialized = 0;
    op_sound_mode = 51;
    calls = 0;
    check(op_sound_play_name(0, 0xffffffffU) == -1,
          "uninitialized returns minus one");
    check(calls == 0 && op_sound_mode == 51,
          "uninitialized path invokes nothing and preserves mode");
    active(1, name, 0U, 0);
    active(-3, name, 0xffffffffU, INT_MIN);
    active(2, 0, 0x800U, INT_MAX);
    printf("Sound play name: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
