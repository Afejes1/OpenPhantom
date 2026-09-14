#include "../src/ui_widget_control.h"
int op_ui_debug_boxes, op_ui_mouse_focus_clears, op_ui3d_active;
#include "../src/ui_widget_control.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u820_checks, u820_failures;
static void u820_check(int ok, int line)
{
    ++u820_checks;
    if (!ok)
    {
        ++u820_failures;
        printf("line%d failed\n", line);
    }
}
#define U820_CHECK(x) u820_check(!!(x), __LINE__)
typedef struct U820_OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} U820_OWNED_WIDGET;
static U820_OWNED_WIDGET u820_widgets[2], u820_expected_widgets[2];
static int u820_expected_debug, u820_expected_mouse, u820_expected_active;
static void u820_seed(int u820_pattern)
{
    memset(u820_widgets, 0x69, sizeof(u820_widgets));
    u820_widgets[0].value.id = u820_pattern * 13;
    u820_widgets[1].value.id = -u820_pattern - 1;
    op_ui_debug_boxes = u820_pattern - 4;
    op_ui_mouse_focus_clears = u820_pattern * 7;
    op_ui3d_active = u820_pattern & 1 ? 1 : -1;
    memcpy(u820_expected_widgets, u820_widgets, sizeof(u820_widgets));
    u820_expected_debug = op_ui_debug_boxes;
    u820_expected_mouse = op_ui_mouse_focus_clears;
    u820_expected_active = op_ui3d_active;
}
static void u820_verify(void)
{
    U820_CHECK(memcmp(u820_widgets, u820_expected_widgets, sizeof(u820_widgets)) == 0);
    U820_CHECK(op_ui_debug_boxes == u820_expected_debug);
    U820_CHECK(op_ui_mouse_focus_clears == u820_expected_mouse);
    U820_CHECK(op_ui3d_active == u820_expected_active);
}
static int op_test_ui_toggle_debug_boxes(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, u820_pattern, repeat;

    for (a = 0; a < 7; ++a)
        for (u820_pattern = 0; u820_pattern < 8; ++u820_pattern)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                u820_seed(u820_pattern);
                op_ui_debug_boxes = values[a];
                u820_expected_debug = values[a] == 0;
                op_ui_toggle_debug_boxes();
                u820_verify();
            }
    printf("ui_toggle_debug_boxes: %d checks, %d failures\n", u820_checks, u820_failures);
    return u820_failures != 0;
}

#undef U820_CHECK

#include "../src/ui_widget_control.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u821_checks, u821_failures;
static void u821_check(int ok, int line)
{
    ++u821_checks;
    if (!ok)
    {
        ++u821_failures;
        printf("line%d failed\n", line);
    }
}
#define U821_CHECK(x) u821_check(!!(x), __LINE__)
typedef struct U821_OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} U821_OWNED_WIDGET;
static U821_OWNED_WIDGET u821_widgets[2], u821_expected_widgets[2];
static int u821_expected_debug, u821_expected_mouse, u821_expected_active;
static void u821_seed(int u821_pattern)
{
    memset(u821_widgets, 0x69, sizeof(u821_widgets));
    u821_widgets[0].value.id = u821_pattern * 13;
    u821_widgets[1].value.id = -u821_pattern - 1;
    op_ui_debug_boxes = u821_pattern - 4;
    op_ui_mouse_focus_clears = u821_pattern * 7;
    op_ui3d_active = u821_pattern & 1 ? 1 : -1;
    memcpy(u821_expected_widgets, u821_widgets, sizeof(u821_widgets));
    u821_expected_debug = op_ui_debug_boxes;
    u821_expected_mouse = op_ui_mouse_focus_clears;
    u821_expected_active = op_ui3d_active;
}
static void u821_verify(void)
{
    U821_CHECK(memcmp(u821_widgets, u821_expected_widgets, sizeof(u821_widgets)) == 0);
    U821_CHECK(op_ui_debug_boxes == u821_expected_debug);
    U821_CHECK(op_ui_mouse_focus_clears == u821_expected_mouse);
    U821_CHECK(op_ui3d_active == u821_expected_active);
}
static int op_test_ui_set_mouse_focus_clears(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, u821_pattern, repeat;

    for (a = 0; a < 7; ++a)
        for (u821_pattern = 0; u821_pattern < 8; ++u821_pattern)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                u821_seed(u821_pattern);
                u821_expected_mouse = values[a];
                op_ui_set_mouse_focus_clears(values[a]);
                u821_verify();
            }
    printf("ui_set_mouse_focus_clears: %d checks, %d failures\n", u821_checks, u821_failures);
    return u821_failures != 0;
}

#undef U821_CHECK

#include "../src/ui_widget_control.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u822_checks, u822_failures;
static void u822_check(int ok, int line)
{
    ++u822_checks;
    if (!ok)
    {
        ++u822_failures;
        printf("line%d failed\n", line);
    }
}
#define U822_CHECK(x) u822_check(!!(x), __LINE__)
typedef struct U822_OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} U822_OWNED_WIDGET;
static U822_OWNED_WIDGET u822_widgets[2], u822_expected_widgets[2];
static int u822_expected_debug, u822_expected_mouse, u822_expected_active;
static void u822_seed(int u822_pattern)
{
    memset(u822_widgets, 0x69, sizeof(u822_widgets));
    u822_widgets[0].value.id = u822_pattern * 13;
    u822_widgets[1].value.id = -u822_pattern - 1;
    op_ui_debug_boxes = u822_pattern - 4;
    op_ui_mouse_focus_clears = u822_pattern * 7;
    op_ui3d_active = u822_pattern & 1 ? 1 : -1;
    memcpy(u822_expected_widgets, u822_widgets, sizeof(u822_widgets));
    u822_expected_debug = op_ui_debug_boxes;
    u822_expected_mouse = op_ui_mouse_focus_clears;
    u822_expected_active = op_ui3d_active;
}
static void u822_verify(void)
{
    U822_CHECK(memcmp(u822_widgets, u822_expected_widgets, sizeof(u822_widgets)) == 0);
    U822_CHECK(op_ui_debug_boxes == u822_expected_debug);
    U822_CHECK(op_ui_mouse_focus_clears == u822_expected_mouse);
    U822_CHECK(op_ui3d_active == u822_expected_active);
}
static int op_test_ui_widget_activation_id(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, u822_pattern, repeat;
    int result;
    for (a = 0; a < 7; ++a)
        for (u822_pattern = 0; u822_pattern < 8; ++u822_pattern)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                u822_seed(u822_pattern);
                u822_widgets[u822_pattern & 1].value.id = u822_expected_widgets[u822_pattern & 1].value.id = values[a];
                result = op_ui_widget_activation_id(&u822_widgets[u822_pattern & 1].value);
                U822_CHECK(result == values[a]);
                u822_verify();
            }
    printf("ui_widget_activation_id: %d checks, %d failures\n", u822_checks, u822_failures);
    return u822_failures != 0;
}

#undef U822_CHECK

#include "../src/ui_widget_control.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u823_checks, u823_failures;
static void u823_check(int ok, int line)
{
    ++u823_checks;
    if (!ok)
    {
        ++u823_failures;
        printf("line%d failed\n", line);
    }
}
#define U823_CHECK(x) u823_check(!!(x), __LINE__)
typedef struct U823_OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} U823_OWNED_WIDGET;
static U823_OWNED_WIDGET u823_widgets[2], u823_expected_widgets[2];
static int u823_expected_debug, u823_expected_mouse, u823_expected_active;
static void u823_seed(int u823_pattern)
{
    memset(u823_widgets, 0x69, sizeof(u823_widgets));
    u823_widgets[0].value.id = u823_pattern * 13;
    u823_widgets[1].value.id = -u823_pattern - 1;
    op_ui_debug_boxes = u823_pattern - 4;
    op_ui_mouse_focus_clears = u823_pattern * 7;
    op_ui3d_active = u823_pattern & 1 ? 1 : -1;
    memcpy(u823_expected_widgets, u823_widgets, sizeof(u823_widgets));
    u823_expected_debug = op_ui_debug_boxes;
    u823_expected_mouse = op_ui_mouse_focus_clears;
    u823_expected_active = op_ui3d_active;
}
static void u823_verify(void)
{
    U823_CHECK(memcmp(u823_widgets, u823_expected_widgets, sizeof(u823_widgets)) == 0);
    U823_CHECK(op_ui_debug_boxes == u823_expected_debug);
    U823_CHECK(op_ui_mouse_focus_clears == u823_expected_mouse);
    U823_CHECK(op_ui3d_active == u823_expected_active);
}
static int op_test_ui3d_begin(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, u823_pattern, repeat;

    for (a = 0; a < 7; ++a)
        for (u823_pattern = 0; u823_pattern < 8; ++u823_pattern)
            for (repeat = 0; repeat < 4; ++repeat)
            {
                u823_seed(u823_pattern);
                op_ui3d_active = values[a];
                u823_expected_active = values[a] ? values[a] : 1;
                op_ui3d_begin();
                u823_verify();
            }
    printf("ui3d_begin: %d checks, %d failures\n", u823_checks, u823_failures);
    return u823_failures != 0;
}

#undef U823_CHECK

#include "../src/ui_widget_control.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int u824_checks, u824_failures;
static void u824_check(int ok, int line)
{
    ++u824_checks;
    if (!ok)
    {
        ++u824_failures;
        printf("line%d failed\n", line);
    }
}
#define U824_CHECK(x) u824_check(!!(x), __LINE__)
typedef struct U824_OWNED_WIDGET
{
    unsigned int before;
    OP_UI_WIDGET_PREFIX value;
    unsigned int trailing[5];
} U824_OWNED_WIDGET;
static U824_OWNED_WIDGET u824_widgets[2], u824_expected_widgets[2];
static int u824_expected_debug, u824_expected_mouse, u824_expected_active;
static void u824_seed(int u824_pattern)
{
    memset(u824_widgets, 0x69, sizeof(u824_widgets));
    u824_widgets[0].value.id = u824_pattern * 13;
    u824_widgets[1].value.id = -u824_pattern - 1;
    op_ui_debug_boxes = u824_pattern - 4;
    op_ui_mouse_focus_clears = u824_pattern * 7;
    op_ui3d_active = u824_pattern & 1 ? 1 : -1;
    memcpy(u824_expected_widgets, u824_widgets, sizeof(u824_widgets));
    u824_expected_debug = op_ui_debug_boxes;
    u824_expected_mouse = op_ui_mouse_focus_clears;
    u824_expected_active = op_ui3d_active;
}
static void u824_verify(void)
{
    U824_CHECK(memcmp(u824_widgets, u824_expected_widgets, sizeof(u824_widgets)) == 0);
    U824_CHECK(op_ui_debug_boxes == u824_expected_debug);
    U824_CHECK(op_ui_mouse_focus_clears == u824_expected_mouse);
    U824_CHECK(op_ui3d_active == u824_expected_active);
}

static int u824_calls, u824_wanted_calls, u824_pattern, u824_mutate, u824_queue_result;
static void u824_observe(int stage)
{
    U824_CHECK(stage == u824_calls);
    U824_CHECK(u824_calls < u824_wanted_calls);
    if (stage != u824_calls || u824_calls >= u824_wanted_calls)
        exit(2);
    u824_verify();
    ++u824_calls;
    if (u824_mutate)
    {
        op_ui3d_active = u824_expected_active = (u824_pattern + stage) % 4 - 1;
        op_ui_debug_boxes = u824_expected_debug = stage * 31 - u824_pattern;
        op_ui_mouse_focus_clears = u824_expected_mouse = stage * 17 + u824_pattern;
        u824_widgets[stage & 1].value.id = u824_expected_widgets[stage & 1].value.id = stage * 13 - u824_pattern;
    }
}
void op_ui3d_flush_material_buckets(void)
{
    u824_observe(0);
}
int op_ui3d_flush_draw_queue(void)
{
    u824_observe(1);
    return u824_queue_result;
}
void op_ui3d_finish_backend(void)
{
    u824_observe(2);
}
static int op_test_ui3d_end(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, r;
    for (a = 0; a < 7; ++a)
        for (r = 0; r < 7; ++r)
            for (u824_pattern = 0; u824_pattern < 8; ++u824_pattern)
                for (u824_mutate = 0; u824_mutate < 2; ++u824_mutate)
                {
                    u824_seed(u824_pattern);
                    op_ui3d_active = u824_expected_active = values[a];
                    u824_queue_result = values[r];
                    u824_calls = 0;
                    u824_wanted_calls = values[a] == 1 ? 3 : 0;
                    op_ui3d_end();
                    if (u824_wanted_calls)
                        u824_expected_active = 0;
                    U824_CHECK(u824_calls == u824_wanted_calls);
                    u824_verify();
                }
    printf("ui3d_end: %d checks, %d failures\n", u824_checks, u824_failures);
    return u824_failures != 0;
}

#undef U824_CHECK

static int op_test_ui_widget_connected(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, cycle, expected_calls, next_mouse, before_checks = u824_checks, before_failures = u824_failures;
    for (a = 0; a < 7; ++a)
        for (u824_pattern = 0; u824_pattern < 8; ++u824_pattern)
            for (u824_mutate = 0; u824_mutate < 2; ++u824_mutate)
            {
                u824_seed(u824_pattern);
                op_ui3d_active = u824_expected_active = values[a];
                u824_queue_result = values[(a + 3) % 7];
                for (cycle = 0; cycle < 2; ++cycle)
                {
                    if (!u824_expected_active)
                        u824_expected_active = 1;
                    op_ui3d_begin();
                    u824_verify();
                    expected_calls = u824_expected_active == 1 ? 3 : 0;
                    u824_wanted_calls = expected_calls;
                    u824_calls = 0;
                    op_ui3d_end();
                    if (expected_calls)
                        u824_expected_active = 0;
                    u824_check(u824_calls == expected_calls, __LINE__);
                    u824_verify();
                    u824_expected_debug = !u824_expected_debug;
                    op_ui_toggle_debug_boxes();
                    next_mouse = ~u824_expected_mouse;
                    u824_expected_mouse = next_mouse;
                    op_ui_set_mouse_focus_clears(next_mouse);
                    u824_check(op_ui_widget_activation_id(&u824_widgets[u824_pattern & 1].value) ==
                                   u824_expected_widgets[u824_pattern & 1].value.id,
                               __LINE__);
                    u824_verify();
                }
            }
    printf("UI widget connected: %d checks, %d failures\n", u824_checks - before_checks,
           u824_failures - before_failures);
    return u824_failures != before_failures;
}
