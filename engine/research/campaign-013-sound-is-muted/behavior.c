#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_sound_disabled;
int op_sound_initialized;
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
    static const int disabled_values[] = { 1, 0, -1, 2, INT_MIN, INT_MAX };
    static const int initialized_values[] = { 0, 1, -1, INT_MIN };
    static const int truth[6][4] = {
        {1,1,1,1},{1,0,0,0},{1,0,0,0},
        {1,0,0,0},{1,0,0,0},{1,0,0,0}
    };
    unsigned int i;
    unsigned int j;

    for (i = 0; i < sizeof(disabled_values) / sizeof(disabled_values[0]); ++i) {
        for (j = 0; j < sizeof(initialized_values) / sizeof(initialized_values[0]); ++j) {
            int disabled = disabled_values[i];
            int initialized = initialized_values[j];
            int expected = truth[i][j];
            int first;
            int second;

            op_sound_disabled = disabled;
            op_sound_initialized = initialized;
            first = op_sound_is_muted();
            second = op_sound_is_muted();
            check(first == expected, "truth-table result");
            check(second == expected, "repeated result");
            check(op_sound_disabled == disabled, "disabled global preserved");
            check(op_sound_initialized == initialized, "initialized global preserved");
        }
    }

    printf("Sound muted: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
