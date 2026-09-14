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

static int calls, wanted_calls, pattern, mutate, queue_result;
static void observe(int stage)
{
    CHECK(stage == calls);
    CHECK(calls < wanted_calls);
    if (stage != calls || calls >= wanted_calls)
        exit(2);
    verify();
    ++calls;
    if (mutate)
    {
        op_ui3d_active = expected_active = (pattern + stage) % 4 - 1;
        op_ui_debug_boxes = expected_debug = stage * 31 - pattern;
        op_ui_mouse_focus_clears = expected_mouse = stage * 17 + pattern;
        widgets[stage & 1].value.id = expected_widgets[stage & 1].value.id = stage * 13 - pattern;
    }
}
void op_ui3d_flush_material_buckets(void)
{
    observe(0);
}
int op_ui3d_flush_draw_queue(void)
{
    observe(1);
    return queue_result;
}
void op_ui3d_finish_backend(void)
{
    observe(2);
}
int main(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, r;
    for (a = 0; a < 7; ++a)
        for (r = 0; r < 7; ++r)
            for (pattern = 0; pattern < 8; ++pattern)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    seed(pattern);
                    op_ui3d_active = expected_active = values[a];
                    queue_result = values[r];
                    calls = 0;
                    wanted_calls = values[a] == 1 ? 3 : 0;
                    op_ui3d_end();
                    if (wanted_calls)
                        expected_active = 0;
                    CHECK(calls == wanted_calls);
                    verify();
                }
    printf("ui3d_end: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
