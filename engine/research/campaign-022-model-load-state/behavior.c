#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
typedef struct GUARDED
{
    unsigned int before;
    OP_MODEL model;
    unsigned int after;
} GUARDED;
static GUARDED state, expected;
static int checks, failures;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}
int main(void)
{
    static const int values[] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
    unsigned int i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        memset(&state, 0x6d, sizeof(state));
        state.before = 0x11223344U;
        state.after = 0x55667788U;
        state.model.load_state = values[i];
        expected = state;
        check(op_model_load_state(&state.model) == values[i]);
        check(memcmp(&state, &expected, sizeof(state)) == 0);
        check(op_model_load_state(&state.model) == values[i]);
        check(memcmp(&state, &expected, sizeof(state)) == 0);
    }
    printf("model load state: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
