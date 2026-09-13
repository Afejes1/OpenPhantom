#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_music_initialized;
int op_music_state;
static int checks;
static int failures;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void expect(int initialized, int state, int expected, const char *description)
{
    op_music_initialized = initialized;
    op_music_state = state;
    check(op_music_get_state() == expected, description);
    check(op_music_initialized == initialized && op_music_state == state,
          "query does not mutate globals");
}

int main(void)
{
    expect(0, 7, 1000, "uninitialized sentinel");
    expect(1, -13, -13, "initialized returns negative state unchanged");
    expect(-1, INT_MAX, INT_MAX, "negative initialized value is active");
    expect(27, INT_MIN, INT_MIN, "boundary state returned unchanged");
    printf("Music state: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}