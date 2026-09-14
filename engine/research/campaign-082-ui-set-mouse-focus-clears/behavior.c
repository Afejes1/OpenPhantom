#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int op_ui_debug_boxes, op_ui_mouse_focus_clears, op_ui3d_active;
static int checks, failures;
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
typedef struct OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} OWNED_WIDGET;
static OWNED_WIDGET widgets[2], expected_widgets[2];
static int expected_debug, expected_mouse, expected_active;
static void seed(int pattern)
{
    memset(widgets, 0x69, sizeof(widgets));
    widgets[0].value.id = pattern * 13;
    widgets[1].value.id = -pattern - 1;
    op_ui_debug_boxes = pattern - 4;
    op_ui_mouse_focus_clears = pattern * 7;
    op_ui3d_active = pattern & 1 ? 1 : -1;
    memcpy(expected_widgets, widgets, sizeof(widgets));
    expected_debug = op_ui_debug_boxes;
    expected_mouse = op_ui_mouse_focus_clears;
    expected_active = op_ui3d_active;
}
static void verify(void)
{
    CHECK(memcmp(widgets, expected_widgets, sizeof(widgets)) == 0);
    CHECK(op_ui_debug_boxes == expected_debug);
    CHECK(op_ui_mouse_focus_clears == expected_mouse);
    CHECK(op_ui3d_active == expected_active);
}
int main(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, pattern, repeat;

    for (a = 0; a < 7; ++a)
        for (pattern = 0; pattern < 8; ++pattern)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                seed(pattern);
                expected_mouse = values[a];
                op_ui_set_mouse_focus_clears(values[a]);
                verify();
            }
    printf("ui_set_mouse_focus_clears: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
