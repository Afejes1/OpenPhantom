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

int op_letterbox_visible, op_letterbox_target, op_letterbox_previous, op_letterbox_step;
OP_OVERLAY_SAVE op_overlay_save;
static OP_OVERLAY_SAVE expected;
static int live[4], calls;
static unsigned char side[16], expected_side[16];
static void verify_state(void)
{
    CHECK(memcmp(&op_overlay_save, &expected, sizeof(expected)) == 0);
    CHECK(op_letterbox_visible == live[0] && op_letterbox_target == live[1] && op_letterbox_previous == live[2] &&
          op_letterbox_step == live[3]);
    CHECK(memcmp(side, expected_side, sizeof(side)) == 0);
}
static void initialize(int n)
{
    int i;
    memset(&op_overlay_save, 0x39 + n, sizeof(op_overlay_save));
    memset(side, 0x52, sizeof(side));
    op_letterbox_visible = n % 2 ? INT_MIN : 0;
    op_letterbox_target = n % 2 ? -1 : 1;
    op_letterbox_previous = INT_MAX - n;
    op_letterbox_step = -7 + n;
    live[0] = op_letterbox_visible;
    live[1] = op_letterbox_target;
    live[2] = op_letterbox_previous;
    live[3] = op_letterbox_step;
    expected = op_overlay_save;
    memcpy(expected_side, side, sizeof(side));
    calls = 0;
    for (i = 0; i < 12; ++i)
        CHECK(expected.prefix[i] == (unsigned char)(0x39 + n));
}

int main(void)
{
    static const int values[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int i;
    for (i = 0; i < 5; ++i)
    {
        initialize(i);
        op_overlay_save.visible = values[i];
        op_overlay_save.target = values[(i + 1) % 5];
        op_overlay_save.previous = values[(i + 2) % 5];
        op_overlay_save.step = values[(i + 3) % 5];
        expected = op_overlay_save;
        live[0] = expected.visible;
        live[1] = expected.target;
        live[2] = expected.previous;
        live[3] = expected.step;
        CHECK(op_overlay_restore_state() == 0);
        CHECK(calls == 0);
        verify_state();
    }
    printf("overlay restore body: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
