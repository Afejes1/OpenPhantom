#include "api.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

int op_sound_load_failures;
static int checks, failures, calls;
static op_sound_ref *current;
static void *callback_result;
static int callback_counter;
static char expected_name[52];
static char resources[4];

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", description); }
}

void *op_sound_acquire_resource(unsigned int type, char *name)
{
    ++calls;
    check(type == 0x42534e44U, "BSND tag forwarded");
    check(strcmp(name, expected_name) == 0, "independent stack name copy forwarded");
    check(name != current->name, "acquisition receives independent name storage");
    if (expected_name[0] != 0)
        current->name[0] = 'X';
    check(strcmp(name, expected_name) == 0,
          "record name mutation does not change the copied name");
    current->resource = &resources[1];
    op_sound_load_failures = callback_counter;
    return callback_result;
}

static void run_case(const char *name, void *result, int before_counter,
                     int callback_value, int expected_return,
                     int expected_counter)
{
    op_sound_ref record;
    memset(&record, 0x55, sizeof record);
    memset(record.name, 0, sizeof record.name);
    strcpy(record.name, name);
    record.resource = 0;
    strcpy(expected_name, name);
    current = &record;
    callback_result = result;
    callback_counter = callback_value;
    op_sound_load_failures = before_counter;
    calls = 0;
    check(op_sound_load_ref(&record) == expected_return, "load return value");
    check(calls == 1, "one acquire callback");
    check(record.resource == result, "callback resource mutation overwritten by return");
    check(op_sound_load_failures == expected_counter, "failure counter transition");
}

int main(void)
{
    op_sound_ref record;
    char long_name[52];
    int i;
    check(offsetof(op_sound_ref, name) == 4 &&
          offsetof(op_sound_ref, resource) == 0x38, "reference ABI");
    op_sound_load_failures = 7;
    calls = 0;
    check(op_sound_load_ref(0) == 0, "null record returns zero");
    check(calls == 0 && op_sound_load_failures == 7, "null record is untouched");
    memset(&record, 0, sizeof record);
    record.resource = &resources[0];
    check(op_sound_load_ref(&record) == 1 && calls == 0,
          "loaded record returns one without callback");
    run_case("", &resources[2], 3, 11, 1, 11);
    run_case("ordinary", &resources[3], 3, -8, 1, -8);
    for (i = 0; i < 51; ++i)
        long_name[i] = (char)('a' + i % 26);
    long_name[51] = 0;
    run_case(long_name, &resources[2], 0, 6, 1, 6);
    run_case("fail19", 0, 2, 19, 0, 20);
    run_case("fail20", 0, 2, 20, 0, 0);
    run_case("negative", 0, 2, -3, 0, -2);
    printf("Sound load ref: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
