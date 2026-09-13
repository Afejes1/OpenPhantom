#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_overlay_save_state_checks, hl_overlay_save_state_failures;
static void hl_overlay_save_state_check_at(int value, int hl_overlay_save_state_line)
{
    ++hl_overlay_save_state_checks;
    if (!value)
    {
        ++hl_overlay_save_state_failures;
        printf("failed at %d\n", hl_overlay_save_state_line);
    }
}
#hl_overlay_save_state_define hl_overlay_save_state_CHECK(hl_overlay_save_state_x)                                     \
    hl_overlay_save_state_check_at(!!(hl_overlay_save_state_x), __LINE__)

static OP_OVERLAY_SAVE hl_overlay_save_state_expected;
static int hl_overlay_save_state_live[4], hl_overlay_save_state_calls;
static unsigned char hl_overlay_save_state_side[16], hl_overlay_save_state_expected_side[16];
static void hl_overlay_save_state_verify_state(void)
{
    hl_overlay_save_state_CHECK(
        memcmp(&op_overlay_save, &hl_overlay_save_state_expected, sizeof(hl_overlay_save_state_expected)) == 0);
    hl_overlay_save_state_CHECK(
        op_letterbox_visible == hl_overlay_save_state_live[0] && op_letterbox_target == hl_overlay_save_state_live[1] &&
        op_letterbox_previous == hl_overlay_save_state_live[2] && op_letterbox_step == hl_overlay_save_state_live[3]);
    hl_overlay_save_state_CHECK(memcmp(hl_overlay_save_state_side, hl_overlay_save_state_expected_side,
                                       sizeof(hl_overlay_save_state_side)) == 0);
}
static void hl_overlay_save_state_initialize(int hl_overlay_save_state_n)
{
    int hl_overlay_save_state_i;
    memset(&op_overlay_save, 0x39 + hl_overlay_save_state_n, sizeof(op_overlay_save));
    memset(hl_overlay_save_state_side, 0x52, sizeof(hl_overlay_save_state_side));
    op_letterbox_visible = hl_overlay_save_state_n % 2 ? INT_MIN : 0;
    op_letterbox_target = hl_overlay_save_state_n % 2 ? -1 : 1;
    op_letterbox_previous = INT_MAX - hl_overlay_save_state_n;
    op_letterbox_step = -7 + hl_overlay_save_state_n;
    hl_overlay_save_state_live[0] = op_letterbox_visible;
    hl_overlay_save_state_live[1] = op_letterbox_target;
    hl_overlay_save_state_live[2] = op_letterbox_previous;
    hl_overlay_save_state_live[3] = op_letterbox_step;
    hl_overlay_save_state_expected = op_overlay_save;
    memcpy(hl_overlay_save_state_expected_side, hl_overlay_save_state_side, sizeof(hl_overlay_save_state_side));
    hl_overlay_save_state_calls = 0;
    for (hl_overlay_save_state_i = 0; hl_overlay_save_state_i < 12; ++hl_overlay_save_state_i)
        hl_overlay_save_state_CHECK(hl_overlay_save_state_expected.prefix[hl_overlay_save_state_i] ==
                                    (unsigned char)(0x39 + hl_overlay_save_state_n));
}
static void hl_overlay_save_state_mutate(void)
{
    memset(&op_overlay_save, 0x79, sizeof(op_overlay_save));
    hl_overlay_save_state_expected = op_overlay_save;
    op_letterbox_visible = -31;
    op_letterbox_target = 37;
    op_letterbox_previous = -41;
    op_letterbox_step = 43;
    hl_overlay_save_state_live[0] = -31;
    hl_overlay_save_state_live[1] = 37;
    hl_overlay_save_state_live[2] = -41;
    hl_overlay_save_state_live[3] = 43;
    hl_overlay_save_state_side[7] = 0x99;
    hl_overlay_save_state_expected_side[7] = 0x99;
}

void hl_overlay_save_state_op_save_write(const void *hl_overlay_save_state_memory,
                                         unsigned int hl_overlay_save_state_bytes)
{
    hl_overlay_save_state_CHECK(hl_overlay_save_state_calls++ == 0);
    hl_overlay_save_state_CHECK(hl_overlay_save_state_memory == &op_overlay_save && hl_overlay_save_state_bytes == 28);
    hl_overlay_save_state_verify_state();
    hl_overlay_save_state_mutate();
}
int hl_overlay_save_state_main(void)
{
    int hl_overlay_save_state_i;
    for (hl_overlay_save_state_i = 0; hl_overlay_save_state_i < 5; ++hl_overlay_save_state_i)
    {
        hl_overlay_save_state_initialize(hl_overlay_save_state_i);
        hl_overlay_save_state_expected.visible = hl_overlay_save_state_live[0];
        hl_overlay_save_state_expected.target = hl_overlay_save_state_live[1];
        hl_overlay_save_state_expected.previous = hl_overlay_save_state_live[2];
        hl_overlay_save_state_expected.step = hl_overlay_save_state_live[3];
        hl_overlay_save_state_CHECK(op_overlay_save_state() == 0);
        hl_overlay_save_state_CHECK(hl_overlay_save_state_calls == 1);
        hl_overlay_save_state_verify_state();
    }
    printf("overlay save: %d checks, %d failures\n", hl_overlay_save_state_checks, hl_overlay_save_state_failures);
    return hl_overlay_save_state_failures != 0;
}

#undef hl_overlay_save_state_CHECK
