#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_music_initialized;
int op_music_sequence;
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

static void expect(int initialized, int sequence, int expected, const char *description)
{
    op_music_initialized = initialized;
    op_music_sequence = sequence;
    check(op_music_get_sequence() == expected, description);
    check(op_music_initialized == initialized && op_music_sequence == sequence,
          "query does not mutate globals");
}

int main(void)
{
    expect(0, 17, 2000, "uninitialized sentinel");
    expect(1, -31, -31, "initialized returns negative sequence unchanged");
    expect(-1, INT_MAX, INT_MAX, "negative initialized value is active");
    expect(19, INT_MIN, INT_MIN, "boundary sequence returned unchanged");
    printf("Music sequence: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}