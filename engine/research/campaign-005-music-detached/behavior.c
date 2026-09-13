#include "api.h"
#include <stdio.h>
#include <limits.h>

int op_music_initialized;
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

int main(void)
{
    const int values[] = {0, 1, -1, 73, INT_MIN, INT_MAX};
    int i;
    for (i = 0; i < 6; ++i) {
        op_music_initialized = values[i];
        check(op_music_is_detached() == (values[i] == 0),
              "normalized attachment status");
        check(op_music_initialized == values[i], "getter does not mutate state");
    }
    printf("Music detached: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}