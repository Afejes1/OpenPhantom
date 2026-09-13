#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

typedef struct STATE
{
    unsigned int before;
    int values[32];
    unsigned int after;
} STATE;
static STATE state, expected;
static int calls, control;
int op_shield_destroy(int slot)
{
    CHECK(slot == calls);
    CHECK(slot >= 0 && slot < 32);
    CHECK(memcmp(&state, &expected, sizeof(state)) == 0);
    if (slot < 0 || slot >= 32)
        return -99;
    state.values[slot] = slot * 7 - 9;
    expected.values[slot] = slot * 7 - 9;
    control = 31 - slot;
    ++calls;
    return slot % 3 == 0 ? -7 : (slot % 3 == 1 ? 0 : control);
}
int main(void)
{
    memset(&state, 0x66, sizeof(state));
    expected = state;
    op_shield_destroy_all();
    CHECK(calls == 32);
    CHECK(control == 0);
    CHECK(memcmp(&state, &expected, sizeof(state)) == 0);
    calls = 0;
    op_shield_destroy_all();
    CHECK(calls == 32);
    CHECK(memcmp(&state, &expected, sizeof(state)) == 0);
    printf("shield destroy all: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
