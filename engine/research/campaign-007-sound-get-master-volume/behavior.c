#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_sound_initialized;
void *op_sound_driver;
static int checks;
static int failures;
static int calls;
static int callback_result;
static void *expected_driver;
static char first_driver;
static char second_driver;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static int __stdcall query_stub(void *driver)
{
    ++calls;
    check(driver == expected_driver, "current driver forwarded exactly");
    op_sound_driver = &second_driver;
    op_sound_initialized = 73;
    return callback_result;
}

op_sound_query_volume_fn op_sound_query_volume = query_stub;

static void gated(void)
{
    calls = 0;
    op_sound_initialized = 0;
    op_sound_driver = &first_driver;
    check(op_sound_get_master_volume() == 0, "uninitialized returns zero");
    check(calls == 0, "uninitialized performs no callback");
    check(op_sound_driver == &first_driver && op_sound_initialized == 0,
          "uninitialized preserves globals");
}

static void active(int initialized, void *driver, int result)
{
    calls = 0;
    op_sound_initialized = initialized;
    op_sound_driver = driver;
    expected_driver = driver;
    callback_result = result;
    check(op_sound_get_master_volume() == result,
          "callback return preserved unchanged");
    check(calls == 1, "exactly one query callback");
    check(op_sound_driver == &second_driver && op_sound_initialized == 73,
          "callback global mutations retained");
}

int main(void)
{
    gated();
    active(1, &first_driver, 0);
    active(-1, 0, -17);
    active(2, &first_driver, INT_MAX);
    active(INT_MIN, &first_driver, INT_MIN);
    printf("Sound master volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}