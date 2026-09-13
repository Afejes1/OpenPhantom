#include "api.h"
#include <stdio.h>
#include <string.h>
typedef struct STATE
{
    unsigned int guard0, marker, count, guard1;
} STATE;
static STATE state, expected;
static int checks, failures, step;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}
void op_clear_emitters(void)
{
    check(step == 0);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    step = 1;
    state.marker ^= 0x12345678U;
    state.count = 0;
    expected = state;
}
void op_release_templates(void)
{
    check(step == 1);
    check(memcmp(&state, &expected, sizeof(state)) == 0);
    step = 2;
    state.marker ^= 0xa5a5a5a5U;
    state.guard0 ^= 8;
    expected = state;
}
int main(void)
{
    int i;
    for (i = 0; i < 4; ++i)
    {
        memset(&state, 0x6d, sizeof(state));
        state.count = (unsigned int)(i * 10);
        state.guard0 = 0x11223344U;
        state.guard1 = 0x55667788U;
        expected = state;
        step = 0;
        check(op_emitter_shutdown() == 0);
        check(step == 2);
        check(memcmp(&state, &expected, sizeof(state)) == 0);
    }
    printf("emitter wrapper: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
