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

int op_tint_active, op_tint_mode;
float op_tint_start, op_tint_duration;
int op_letterbox_target, op_letterbox_previous, op_letterbox_step;
static unsigned char unrelated[16], expected_unrelated[16];
static unsigned int bits(const float *v)
{
    unsigned int word;
    memcpy(&word, v, 4);
    return word;
}
int main(void)
{
    static const int active[3] = {0, 1, INT_MIN};
    unsigned int start = 0x7fc00001U, duration = 0x80000000U;
    int i;
    for (i = 0; i < 3; ++i)
    {
        memset(unrelated, 0x51, sizeof(unrelated));
        memcpy(expected_unrelated, unrelated, sizeof(unrelated));
        op_tint_active = active[i];
        op_tint_mode = -7;
        memcpy(&op_tint_start, &start, 4);
        memcpy(&op_tint_duration, &duration, 4);
        op_letterbox_target = -1;
        op_letterbox_previous = INT_MIN;
        op_letterbox_step = INT_MAX;
        op_fade_reset();
        CHECK(op_tint_active == 0);
        CHECK(op_tint_mode == (active[i] ? 0 : -7));
        CHECK(bits(&op_tint_start) == (active[i] ? 0 : start));
        CHECK(bits(&op_tint_duration) == (active[i] ? 0 : duration));
        CHECK(op_letterbox_target == 0 && op_letterbox_previous == 0 && op_letterbox_step == 0);
        CHECK(memcmp(unrelated, expected_unrelated, sizeof(unrelated)) == 0);
    }
    printf("fade reset: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
