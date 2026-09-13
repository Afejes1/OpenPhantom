#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_letterbox_set_enabled_checks, hl_letterbox_set_enabled_failures;
static void hl_letterbox_set_enabled_check_at(int value, int hl_letterbox_set_enabled_line)
{
    ++hl_letterbox_set_enabled_checks;
    if (!value)
    {
        ++hl_letterbox_set_enabled_failures;
        printf("failed at %d\n", hl_letterbox_set_enabled_line);
    }
}
#define hl_letterbox_set_enabled_CHECK(hl_letterbox_set_enabled_x)                            \
    hl_letterbox_set_enabled_check_at(!!(hl_letterbox_set_enabled_x), __LINE__)

typedef struct hl_letterbox_set_enabled_INPUT
{
    unsigned int before;
    int enabled, previous, target, step, visible;
    unsigned int after;
} hl_letterbox_set_enabled_INPUT;
static hl_letterbox_set_enabled_INPUT hl_letterbox_set_enabled_input, hl_letterbox_set_enabled_expected;
static unsigned char hl_letterbox_set_enabled_other[16], hl_letterbox_set_enabled_expected_other[16];
int hl_letterbox_set_enabled_main(void)
{
    static const int hl_letterbox_set_enabled_values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const int hl_letterbox_set_enabled_steps[5] = {-7, 0, 23, 24, INT_MIN};
    int hl_letterbox_set_enabled_a, hl_letterbox_set_enabled_b, hl_letterbox_set_enabled_c, hl_letterbox_set_enabled_d,
        hl_letterbox_set_enabled_wanted;
    for (hl_letterbox_set_enabled_a = 0; hl_letterbox_set_enabled_a < 5; ++hl_letterbox_set_enabled_a)
        for (hl_letterbox_set_enabled_b = 0; hl_letterbox_set_enabled_b < 5; ++hl_letterbox_set_enabled_b)
            for (hl_letterbox_set_enabled_c = 0; hl_letterbox_set_enabled_c < 5; ++hl_letterbox_set_enabled_c)
                for (hl_letterbox_set_enabled_d = 0; hl_letterbox_set_enabled_d < 5; ++hl_letterbox_set_enabled_d)
                {
                    memset(&hl_letterbox_set_enabled_input, 0x61, sizeof(hl_letterbox_set_enabled_input));
                    memset(hl_letterbox_set_enabled_other, 0x42, sizeof(hl_letterbox_set_enabled_other));
                    memcpy(hl_letterbox_set_enabled_expected_other, hl_letterbox_set_enabled_other,
                           sizeof(hl_letterbox_set_enabled_other));
                    hl_letterbox_set_enabled_input.enabled =
                        hl_letterbox_set_enabled_values[hl_letterbox_set_enabled_a];
                    hl_letterbox_set_enabled_input.previous =
                        hl_letterbox_set_enabled_values[hl_letterbox_set_enabled_b];
                    hl_letterbox_set_enabled_input.target = hl_letterbox_set_enabled_values[hl_letterbox_set_enabled_c];
                    hl_letterbox_set_enabled_input.step = hl_letterbox_set_enabled_steps[hl_letterbox_set_enabled_d];
                    hl_letterbox_set_enabled_input.visible = INT_MIN;
                    hl_letterbox_set_enabled_expected = hl_letterbox_set_enabled_input;
                    op_letterbox_visible = hl_letterbox_set_enabled_input.visible;
                    op_letterbox_previous = hl_letterbox_set_enabled_input.previous;
                    op_letterbox_target = hl_letterbox_set_enabled_input.target;
                    op_letterbox_step = hl_letterbox_set_enabled_input.step;
                    hl_letterbox_set_enabled_wanted = hl_letterbox_set_enabled_input.step;
                    if (hl_letterbox_set_enabled_input.enabled && !hl_letterbox_set_enabled_input.previous &&
                        !hl_letterbox_set_enabled_input.target)
                        hl_letterbox_set_enabled_wanted = 23;
                    if (!hl_letterbox_set_enabled_input.enabled && hl_letterbox_set_enabled_input.previous &&
                        hl_letterbox_set_enabled_input.target)
                        hl_letterbox_set_enabled_wanted = 24;
                    op_letterbox_set_enabled(hl_letterbox_set_enabled_input.enabled);
                    hl_letterbox_set_enabled_CHECK(op_letterbox_target == hl_letterbox_set_enabled_input.enabled);
                    hl_letterbox_set_enabled_CHECK(op_letterbox_previous == hl_letterbox_set_enabled_input.previous);
                    hl_letterbox_set_enabled_CHECK(op_letterbox_visible == hl_letterbox_set_enabled_input.visible);
                    hl_letterbox_set_enabled_CHECK(op_letterbox_step == hl_letterbox_set_enabled_wanted);
                    hl_letterbox_set_enabled_CHECK(memcmp(&hl_letterbox_set_enabled_input,
                                                          &hl_letterbox_set_enabled_expected,
                                                          sizeof(hl_letterbox_set_enabled_input)) == 0);
                    hl_letterbox_set_enabled_CHECK(memcmp(hl_letterbox_set_enabled_other,
                                                          hl_letterbox_set_enabled_expected_other,
                                                          sizeof(hl_letterbox_set_enabled_other)) == 0);
                }
    printf("letterbox setter: %d checks, %d failures\n", hl_letterbox_set_enabled_checks,
           hl_letterbox_set_enabled_failures);
    return hl_letterbox_set_enabled_failures != 0;
}

#undef hl_letterbox_set_enabled_CHECK
