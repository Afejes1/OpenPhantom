#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_overlay_size_checks, ah_overlay_size_failures;
static void ah_overlay_size_check(int value, int line)
{
    ah_overlay_size_checks++;
    if (!value)
    {
        ah_overlay_size_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_overlay_size_CHECK(x) ah_overlay_size_check(!!(x), __LINE__)
int ah_overlay_size_main(void)
{
    int i;
    for (i = 0; i < 16; i++)
        ah_overlay_size_CHECK(op_overlay_save_size() == 28);
    printf("overlay size: %d checks, %d failures\n", ah_overlay_size_checks, ah_overlay_size_failures);
    return ah_overlay_size_failures != 0;
}
