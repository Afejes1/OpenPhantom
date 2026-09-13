#include "api.h"
#include <stdio.h>

unsigned int op_view_forced_flags;
static int checks;
static int failures;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void run_case(unsigned int initial, unsigned int mask,
                     unsigned int expected)
{
    op_view_forced_flags = initial;
    op_view_force_flags(mask);
    check(op_view_forced_flags == expected, "explicit OR result");
    op_view_force_flags(mask);
    check(op_view_forced_flags == expected, "idempotent repeat");
}

int main(void)
{
    run_case(0x00000000U, 0x00000000U, 0x00000000U);
    run_case(0x12345678U, 0x00000000U, 0x12345678U);
    run_case(0x00000000U, 0xffffffffU, 0xffffffffU);
    run_case(0x00000001U, 0x80000000U, 0x80000001U);
    run_case(0x55555555U, 0xaaaaaaaaU, 0xffffffffU);
    run_case(0x0f0f0000U, 0x00f0f00fU, 0x0ffff00fU);
    printf("View force flags: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
