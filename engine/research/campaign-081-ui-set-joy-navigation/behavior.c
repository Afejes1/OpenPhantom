#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
OP_UI_CURSOR op_ui_cursor;
OP_UI_JOY_LATCHES op_ui_joy_latches;
int op_ui_navigation_code, op_ui_joy_menu_navigation;
static int checks, failures;
static OP_UI_CURSOR expected_cursor;
static OP_UI_JOY_LATCHES expected_latches;
static int expected_navigation, expected_joy;
static int words[4], expected_words[4];
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static void seed(int pattern)
{
    int i;
    for (i = 0; i < 4; ++i)
        words[i] = (int)(0x12345678u + (unsigned int)pattern * 0x01010101u + (unsigned int)i);
    op_ui_cursor.x = pattern & 1 ? INT_MIN : INT_MAX;
    op_ui_cursor.y = pattern & 2 ? -1 : pattern * 31337;
    op_ui_joy_latches.first = pattern * 11;
    op_ui_joy_latches.second = pattern & 1 ? INT_MIN : 9;
    op_ui_navigation_code = pattern * 17;
    op_ui_joy_menu_navigation = pattern * 23;
    memcpy(expected_words, words, sizeof(words));
    expected_cursor = op_ui_cursor;
    expected_latches = op_ui_joy_latches;
    expected_navigation = op_ui_navigation_code;
    expected_joy = op_ui_joy_menu_navigation;
}
static void verify(void)
{
    CHECK(memcmp(words, expected_words, sizeof(words)) == 0);
    CHECK(memcmp(&op_ui_cursor, &expected_cursor, sizeof(op_ui_cursor)) == 0);
    CHECK(memcmp(&op_ui_joy_latches, &expected_latches, sizeof(op_ui_joy_latches)) == 0);
    CHECK(op_ui_navigation_code == expected_navigation);
    CHECK(op_ui_joy_menu_navigation == expected_joy);
}

int main(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int pattern, a, repeat;
    for (pattern = 0; pattern < 16; ++pattern)
        for (a = 0; a < 7; ++a)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                seed(pattern);
                expected_joy = values[a];
                op_ui_set_joy_navigation(values[a]);
                verify();
            }
    printf("ui_set_joy_navigation: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
