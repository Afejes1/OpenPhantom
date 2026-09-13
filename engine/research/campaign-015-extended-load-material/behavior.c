#include <stdio.h>
#include <string.h>

#include "api.h"

const char op_default_material_name[] = "dflt.mat";

static int checks;
static int failures;
static int mode;
static int calls;
static const char *names[2];
static void *arg1s[2];
static void *arg2s[2];
static void **observed_out;
static unsigned char primary_storage, fallback_storage, initial_storage, mutation_storage;
static void *resource_primary = &primary_storage;
static void *resource_fallback = &fallback_storage;

static void check(int expression)
{
    ++checks;
    if (!expression) {
        ++failures;
    }
}

void *op_load_mat_resource(const char *name, void *arg1, void *arg2)
{
    int call;

    call = calls;
    if (call >= 0 && call < 2) {
        names[call] = name;
        arg1s[call] = arg1;
        arg2s[call] = arg2;
    }
    ++calls;
    if (call == 0) {
        check(*observed_out == &initial_storage);
    }
    if (call == 1) {
        check(*observed_out == 0);
    }
    *observed_out = &mutation_storage;
    if (mode == 0 && call == 0) {
        return resource_primary;
    }
    if (mode == 1 && call == 1) {
        return resource_fallback;
    }
    return 0;
}

typedef struct GUARDED_OUTPUT {
    unsigned int before;
    void *value;
    unsigned int after;
} GUARDED_OUTPUT;

static void run_case(int selected_mode, int expected_result, void *expected_output,
                     int expected_calls)
{
    GUARDED_OUTPUT output;
    const char requested_name[] = "requested.mat";
    int result;
    int i;

    memset(&output, 0x55, sizeof(output));
    output.before = 0x11223344U;
    output.after = 0x55667788U;
    output.value = &initial_storage;
    for (i = 0; i < 2; ++i) {
        names[i] = 0;
        arg1s[i] = &initial_storage;
        arg2s[i] = &initial_storage;
    }
    mode = selected_mode;
    calls = 0;
    observed_out = &output.value;

    result = op_extended_load_material(requested_name, &output.value);

    check(result == expected_result);
    check(output.before == 0x11223344U);
    check(output.after == 0x55667788U);
    check(output.value == expected_output);
    check(calls == expected_calls);
    check(names[0] == requested_name);
    check(arg1s[0] == 0 && arg2s[0] == 0);
    if (expected_calls == 2) {
        check(names[1] == op_default_material_name);
        check(strcmp(names[1], "dflt.mat") == 0);
        check(arg1s[1] == 0 && arg2s[1] == 0);
    }
}

int main(void)
{
    run_case(0, 0, resource_primary, 1);
    run_case(1, 1, resource_fallback, 2);
    run_case(2, 2, 0, 2);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
