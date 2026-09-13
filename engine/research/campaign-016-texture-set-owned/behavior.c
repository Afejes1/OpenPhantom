#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

static int checks;
static int failures;
static int calls;
static void *seen_resource;
static int seen_owned;
static unsigned char callback_state[32];

static void check(int expression)
{
    ++checks;
    if (!expression) {
        ++failures;
    }
}

int op_set_resource_owned(void *resource, int owned)
{
    ++calls;
    seen_resource = resource;
    seen_owned = owned;
    memset(callback_state, 0x6d, sizeof(callback_state));
    return owned == 0 ? 0 : 1;
}

static void run_nonnull(int owned)
{
    unsigned char resource[17];
    unsigned char expected_resource[17];
    unsigned char expected_state[32];

    memset(resource, 0x35, sizeof(resource));
    memcpy(expected_resource, resource, sizeof(resource));
    memset(callback_state, 0x42, sizeof(callback_state));
    memset(expected_state, 0x6d, sizeof(expected_state));
    calls = 0;
    seen_resource = 0;
    seen_owned = 0;

    op_texture_set_owned(resource, owned);

    check(calls == 1);
    check(seen_resource == resource);
    check(seen_owned == owned);
    check(memcmp(resource, expected_resource, sizeof(resource)) == 0);
    check(memcmp(callback_state, expected_state, sizeof(callback_state)) == 0);
}

int main(void)
{
    unsigned char expected_state[32];

    memset(callback_state, 0x27, sizeof(callback_state));
    memcpy(expected_state, callback_state, sizeof(callback_state));
    calls = 0;
    op_texture_set_owned(0, INT_MIN);
    check(calls == 0);
    check(memcmp(callback_state, expected_state, sizeof(callback_state)) == 0);

    run_nonnull(0);
    run_nonnull(1);
    run_nonnull(-7);
    run_nonnull(INT_MIN);
    run_nonnull(INT_MAX);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}
