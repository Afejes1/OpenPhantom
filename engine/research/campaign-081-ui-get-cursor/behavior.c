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

static int *pointer(int index, int expected)
{
    switch (index)
    {
    case 0:
        return expected ? &expected_words[1] : &words[1];
    case 1:
        return expected ? &expected_words[2] : &words[2];
    case 2:
        return expected ? &expected_cursor.x : &op_ui_cursor.x;
    case 3:
        return expected ? &expected_cursor.y : &op_ui_cursor.y;
    case 4:
        return expected ? &expected_navigation : &op_ui_navigation_code;
    case 5:
        return expected ? &expected_joy : &op_ui_joy_menu_navigation;
    case 6:
        return expected ? &expected_latches.first : &op_ui_joy_latches.first;
    default:
        return expected ? &expected_latches.second : &op_ui_joy_latches.second;
    }
}
int main(void)
{
    int a, b, pattern, repeat;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (pattern = 0; pattern < 8; ++pattern)
                for (repeat = 0; repeat < 4; ++repeat)
                {
                    seed(pattern);
                    *pointer(a, 1) = expected_cursor.x;
                    *pointer(b, 1) = expected_cursor.y;
                    op_ui_get_cursor(pointer(a, 0), pointer(b, 0));
                    verify();
                }
    printf("ui_get_cursor: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
