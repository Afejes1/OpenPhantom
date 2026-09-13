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
static int live[4], calls, read_result;
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
static void mutate(void)
{
    memset(&op_overlay_save, 0x79, sizeof(op_overlay_save));
    expected = op_overlay_save;
    op_letterbox_visible = -31;
    op_letterbox_target = 37;
    op_letterbox_previous = -41;
    op_letterbox_step = 43;
    live[0] = -31;
    live[1] = 37;
    live[2] = -41;
    live[3] = 43;
    side[7] = 0x99;
    expected_side[7] = 0x99;
}

int op_save_read(void *memory, unsigned int bytes)
{
    CHECK(calls++ == 0);
    CHECK(memory == &op_overlay_save && bytes == 28);
    verify_state();
    mutate();
    return read_result;
}
int main(void)
{
    static const int results[5] = {0, 1, -1, INT_MIN, INT_MAX};
    int i;
    for (i = 0; i < 5; ++i)
    {
        initialize(i);
        read_result = results[i];
        CHECK(op_overlay_read_state() == (results[i] == 0));
        CHECK(calls == 1);
        verify_state();
    }
    printf("overlay read: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
