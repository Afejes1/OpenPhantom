#include "../src/effects_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ef_fade_reset_checks, ef_fade_reset_failures;
static void ef_fade_reset_check_at(int value, int line)
{
    ++ef_fade_reset_checks;
    if (!value)
    {
        ++ef_fade_reset_failures;
        printf("failed at %d\n", line);
    }
}
#define EF_FADE_RESET_CHECK(x) ef_fade_reset_check_at(!!(x), __LINE__)

static unsigned char ef_fade_reset_unrelated[16], ef_fade_reset_expected_unrelated[16];
static unsigned int ef_fade_reset_bits(const float *v)
{
    unsigned int word;
    memcpy(&word, v, 4);
    return word;
}
static int ef_fade_reset_main(void)
{
    static const int active[3] = {0, 1, INT_MIN};
    unsigned int start = 0x7fc00001U, duration = 0x80000000U;
    int i;
    for (i = 0; i < 3; ++i)
    {
        memset(ef_fade_reset_unrelated, 0x51, sizeof(ef_fade_reset_unrelated));
        memcpy(ef_fade_reset_expected_unrelated, ef_fade_reset_unrelated, sizeof(ef_fade_reset_unrelated));
        op_tint_active = active[i];
        op_tint_mode = -7;
        memcpy(&op_tint_start, &start, 4);
        memcpy(&op_tint_duration, &duration, 4);
        op_letterbox_target = -1;
        op_letterbox_previous = INT_MIN;
        op_letterbox_step = INT_MAX;
        op_fade_reset();
        EF_FADE_RESET_CHECK(op_tint_active == 0);
        EF_FADE_RESET_CHECK(op_tint_mode == (active[i] ? 0 : -7));
        EF_FADE_RESET_CHECK(ef_fade_reset_bits(&op_tint_start) == (active[i] ? 0 : start));
        EF_FADE_RESET_CHECK(ef_fade_reset_bits(&op_tint_duration) == (active[i] ? 0 : duration));
        EF_FADE_RESET_CHECK(op_letterbox_target == 0 && op_letterbox_previous == 0 && op_letterbox_step == 0);
        EF_FADE_RESET_CHECK(
            memcmp(ef_fade_reset_unrelated, ef_fade_reset_expected_unrelated, sizeof(ef_fade_reset_unrelated)) == 0);
    }
    printf("fade reset: %d checks, %d failures\n", ef_fade_reset_checks, ef_fade_reset_failures);
    return ef_fade_reset_failures != 0;
}

#undef EF_FADE_RESET_CHECK
