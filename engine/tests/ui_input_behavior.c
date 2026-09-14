#include "../src/ui_input_state.h"
OP_UI_CURSOR op_ui_cursor;
OP_UI_JOY_LATCHES op_ui_joy_latches;
int op_ui_navigation_code, op_ui_joy_menu_navigation;
#include "../src/ui_input_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u810_checks, u810_failures;
static OP_UI_CURSOR u810_expected_cursor;
static OP_UI_JOY_LATCHES u810_expected_latches;
static int u810_expected_navigation, u810_expected_joy;
static int u810_words[4], u810_expected_words[4];
static void u810_check(int ok, int line)
{
    ++u810_checks;
    if (!ok)
    {
        ++u810_failures;
        printf("line%d failed\n", line);
    }
}
#define U810_CHECK(x) u810_check(!!(x), __LINE__)
static void u810_seed(int pattern)
{
    int i;
    for (i = 0; i < 4; ++i)
        u810_words[i] = (int)(0x12345678u + (unsigned int)pattern * 0x01010101u + (unsigned int)i);
    op_ui_cursor.x = pattern & 1 ? INT_MIN : INT_MAX;
    op_ui_cursor.y = pattern & 2 ? -1 : pattern * 31337;
    op_ui_joy_latches.first = pattern * 11;
    op_ui_joy_latches.second = pattern & 1 ? INT_MIN : 9;
    op_ui_navigation_code = pattern * 17;
    op_ui_joy_menu_navigation = pattern * 23;
    memcpy(u810_expected_words, u810_words, sizeof(u810_words));
    u810_expected_cursor = op_ui_cursor;
    u810_expected_latches = op_ui_joy_latches;
    u810_expected_navigation = op_ui_navigation_code;
    u810_expected_joy = op_ui_joy_menu_navigation;
}
static void u810_verify(void)
{
    U810_CHECK(memcmp(u810_words, u810_expected_words, sizeof(u810_words)) == 0);
    U810_CHECK(memcmp(&op_ui_cursor, &u810_expected_cursor, sizeof(op_ui_cursor)) == 0);
    U810_CHECK(memcmp(&op_ui_joy_latches, &u810_expected_latches, sizeof(op_ui_joy_latches)) == 0);
    U810_CHECK(op_ui_navigation_code == u810_expected_navigation);
    U810_CHECK(op_ui_joy_menu_navigation == u810_expected_joy);
}

static int *u810_pointer(int index, int expected)
{
    switch (index)
    {
    case 0:
        return expected ? &u810_expected_words[1] : &u810_words[1];
    case 1:
        return expected ? &u810_expected_words[2] : &u810_words[2];
    case 2:
        return expected ? &u810_expected_cursor.x : &op_ui_cursor.x;
    case 3:
        return expected ? &u810_expected_cursor.y : &op_ui_cursor.y;
    case 4:
        return expected ? &u810_expected_navigation : &op_ui_navigation_code;
    case 5:
        return expected ? &u810_expected_joy : &op_ui_joy_menu_navigation;
    case 6:
        return expected ? &u810_expected_latches.first : &op_ui_joy_latches.first;
    default:
        return expected ? &u810_expected_latches.second : &op_ui_joy_latches.second;
    }
}
static int op_test_ui_get_cursor(void)
{
    int a, b, pattern, repeat;
    for (a = 0; a < 8; ++a)
        for (b = 0; b < 8; ++b)
            for (pattern = 0; pattern < 8; ++pattern)
                for (repeat = 0; repeat < 4; ++repeat)
                {
                    u810_seed(pattern);
                    *u810_pointer(a, 1) = u810_expected_cursor.x;
                    *u810_pointer(b, 1) = u810_expected_cursor.y;
                    op_ui_get_cursor(u810_pointer(a, 0), u810_pointer(b, 0));
                    u810_verify();
                }
    printf("ui_get_cursor: %d checks, %d failures\n", u810_checks, u810_failures);
    return u810_failures != 0;
}

#undef U810_CHECK

#include "../src/ui_input_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u811_checks, u811_failures;
static OP_UI_CURSOR u811_expected_cursor;
static OP_UI_JOY_LATCHES u811_expected_latches;
static int u811_expected_navigation, u811_expected_joy;
static int u811_words[4], u811_expected_words[4];
static void u811_check(int ok, int line)
{
    ++u811_checks;
    if (!ok)
    {
        ++u811_failures;
        printf("line%d failed\n", line);
    }
}
#define U811_CHECK(x) u811_check(!!(x), __LINE__)
static void u811_seed(int pattern)
{
    int i;
    for (i = 0; i < 4; ++i)
        u811_words[i] = (int)(0x12345678u + (unsigned int)pattern * 0x01010101u + (unsigned int)i);
    op_ui_cursor.x = pattern & 1 ? INT_MIN : INT_MAX;
    op_ui_cursor.y = pattern & 2 ? -1 : pattern * 31337;
    op_ui_joy_latches.first = pattern * 11;
    op_ui_joy_latches.second = pattern & 1 ? INT_MIN : 9;
    op_ui_navigation_code = pattern * 17;
    op_ui_joy_menu_navigation = pattern * 23;
    memcpy(u811_expected_words, u811_words, sizeof(u811_words));
    u811_expected_cursor = op_ui_cursor;
    u811_expected_latches = op_ui_joy_latches;
    u811_expected_navigation = op_ui_navigation_code;
    u811_expected_joy = op_ui_joy_menu_navigation;
}
static void u811_verify(void)
{
    U811_CHECK(memcmp(u811_words, u811_expected_words, sizeof(u811_words)) == 0);
    U811_CHECK(memcmp(&op_ui_cursor, &u811_expected_cursor, sizeof(op_ui_cursor)) == 0);
    U811_CHECK(memcmp(&op_ui_joy_latches, &u811_expected_latches, sizeof(op_ui_joy_latches)) == 0);
    U811_CHECK(op_ui_navigation_code == u811_expected_navigation);
    U811_CHECK(op_ui_joy_menu_navigation == u811_expected_joy);
}

static int op_test_ui_navigation_code_pointer(void)
{
    int pattern, repeat;
    int *result;
    for (pattern = 0; pattern < 32; ++pattern)
        for (repeat = 0; repeat < 4; ++repeat)
        {
            u811_seed(pattern);
            result = op_ui_navigation_code_pointer();
            U811_CHECK(result == &op_ui_navigation_code);
            u811_verify();
            if (result != &op_ui_navigation_code)
                exit(2);
            u811_expected_navigation = -pattern - 1;
            *result = -pattern - 1;
            u811_verify();
        }
    printf("ui_navigation_code_pointer: %d checks, %d failures\n", u811_checks, u811_failures);
    return u811_failures != 0;
}

#undef U811_CHECK

#include "../src/ui_input_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u812_checks, u812_failures;
static OP_UI_CURSOR u812_expected_cursor;
static OP_UI_JOY_LATCHES u812_expected_latches;
static int u812_expected_navigation, u812_expected_joy;
static int u812_words[4], u812_expected_words[4];
static void u812_check(int ok, int line)
{
    ++u812_checks;
    if (!ok)
    {
        ++u812_failures;
        printf("line%d failed\n", line);
    }
}
#define U812_CHECK(x) u812_check(!!(x), __LINE__)
static void u812_seed(int pattern)
{
    int i;
    for (i = 0; i < 4; ++i)
        u812_words[i] = (int)(0x12345678u + (unsigned int)pattern * 0x01010101u + (unsigned int)i);
    op_ui_cursor.x = pattern & 1 ? INT_MIN : INT_MAX;
    op_ui_cursor.y = pattern & 2 ? -1 : pattern * 31337;
    op_ui_joy_latches.first = pattern * 11;
    op_ui_joy_latches.second = pattern & 1 ? INT_MIN : 9;
    op_ui_navigation_code = pattern * 17;
    op_ui_joy_menu_navigation = pattern * 23;
    memcpy(u812_expected_words, u812_words, sizeof(u812_words));
    u812_expected_cursor = op_ui_cursor;
    u812_expected_latches = op_ui_joy_latches;
    u812_expected_navigation = op_ui_navigation_code;
    u812_expected_joy = op_ui_joy_menu_navigation;
}
static void u812_verify(void)
{
    U812_CHECK(memcmp(u812_words, u812_expected_words, sizeof(u812_words)) == 0);
    U812_CHECK(memcmp(&op_ui_cursor, &u812_expected_cursor, sizeof(op_ui_cursor)) == 0);
    U812_CHECK(memcmp(&op_ui_joy_latches, &u812_expected_latches, sizeof(op_ui_joy_latches)) == 0);
    U812_CHECK(op_ui_navigation_code == u812_expected_navigation);
    U812_CHECK(op_ui_joy_menu_navigation == u812_expected_joy);
}

static int op_test_ui_set_joy_navigation(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int pattern, a, repeat;
    for (pattern = 0; pattern < 16; ++pattern)
        for (a = 0; a < 7; ++a)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                u812_seed(pattern);
                u812_expected_joy = values[a];
                op_ui_set_joy_navigation(values[a]);
                u812_verify();
            }
    printf("ui_set_joy_navigation: %d checks, %d failures\n", u812_checks, u812_failures);
    return u812_failures != 0;
}

#undef U812_CHECK

#include "../src/ui_input_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u813_checks, u813_failures;
static OP_UI_CURSOR u813_expected_cursor;
static OP_UI_JOY_LATCHES u813_expected_latches;
static int u813_expected_navigation, u813_expected_joy;
static int u813_words[4], u813_expected_words[4];
static void u813_check(int ok, int line)
{
    ++u813_checks;
    if (!ok)
    {
        ++u813_failures;
        printf("line%d failed\n", line);
    }
}
#define U813_CHECK(x) u813_check(!!(x), __LINE__)
static void u813_seed(int pattern)
{
    int i;
    for (i = 0; i < 4; ++i)
        u813_words[i] = (int)(0x12345678u + (unsigned int)pattern * 0x01010101u + (unsigned int)i);
    op_ui_cursor.x = pattern & 1 ? INT_MIN : INT_MAX;
    op_ui_cursor.y = pattern & 2 ? -1 : pattern * 31337;
    op_ui_joy_latches.first = pattern * 11;
    op_ui_joy_latches.second = pattern & 1 ? INT_MIN : 9;
    op_ui_navigation_code = pattern * 17;
    op_ui_joy_menu_navigation = pattern * 23;
    memcpy(u813_expected_words, u813_words, sizeof(u813_words));
    u813_expected_cursor = op_ui_cursor;
    u813_expected_latches = op_ui_joy_latches;
    u813_expected_navigation = op_ui_navigation_code;
    u813_expected_joy = op_ui_joy_menu_navigation;
}
static void u813_verify(void)
{
    U813_CHECK(memcmp(u813_words, u813_expected_words, sizeof(u813_words)) == 0);
    U813_CHECK(memcmp(&op_ui_cursor, &u813_expected_cursor, sizeof(op_ui_cursor)) == 0);
    U813_CHECK(memcmp(&op_ui_joy_latches, &u813_expected_latches, sizeof(op_ui_joy_latches)) == 0);
    U813_CHECK(op_ui_navigation_code == u813_expected_navigation);
    U813_CHECK(op_ui_joy_menu_navigation == u813_expected_joy);
}

static int op_test_ui_clear_joy_latches(void)
{
    int pattern, repeat;
    for (pattern = 0; pattern < 32; ++pattern)
    {
        u813_seed(pattern);
        u813_expected_latches.first = 0;
        u813_expected_latches.second = 0;
        for (repeat = 0; repeat < 8; ++repeat)
        {
            op_ui_clear_joy_latches();
            u813_verify();
        }
    }
    printf("ui_clear_joy_latches: %d checks, %d failures\n", u813_checks, u813_failures);
    return u813_failures != 0;
}

#undef U813_CHECK
