#include "api.h"
#include <stdio.h>

static int checks;
static int failures;
static int calls;
static int callback_result;
static int callback_state;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_sound_periodic(void)
{
    ++calls;
    callback_state += 7;
    return callback_result;
}

static void run_case(int result)
{
    int before = callback_state;
    callback_result = result;
    calls = 0;
    op_sound_tick();
    check(calls == 1, "one periodic call");
    check(callback_state == before + 7, "callback state retained");
}

int main(void)
{
    run_case(0);
    run_case(-1);
    run_case(23);
    op_sound_tick();
    check(calls == 2, "repeated wrapper call invokes periodic again");
    printf("Sound tick: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}