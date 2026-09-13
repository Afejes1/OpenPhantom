#include "api.h"
#include <limits.h>
#include <stdio.h>

typedef struct DISPATCH_CASE {
    int event;
    int callback;
    int result;
} DISPATCH_CASE;

int op_sound_performance_level;
int op_sound_quality_level;

static int checks;
static int failures;
static int callback_count;
static int expected_callback;
static int expected_quality_at_callback;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static int callback(int id, int result)
{
    check(callback_count == 0, "only one callback per event");
    check(id == expected_callback, "expected callback selected");
    check(op_sound_quality_level == expected_quality_at_callback,
          "callback observes expected quality state");
    ++callback_count;
    op_sound_quality_level = 1000 + id;
    op_sound_performance_level = -1000 - id;
    return result;
}

int op_sound_module_init(void)
{
    return callback(1, 101);
}

int op_sound_shutdown(void)
{
    return callback(2, -202);
}

int op_sound_register_level(void)
{
    return callback(25, 2501);
}

int op_sound_remove_level(int ignored)
{
    check(ignored == 0, "remove-level receives zero ignored argument");
    return callback(6, -606);
}

void op_sound_reset_places(void)
{
    (void)callback(18, 0);
}

void op_sound_stop_all(void)
{
    (void)callback(19, 0);
}

int op_sound_periodic(void)
{
    return callback(14, -1414);
}

void op_sound_pause_all(void)
{
    (void)callback(8, 0);
}

void op_sound_resume_all(void)
{
    (void)callback(9, 0);
}

static void run_case(const DISPATCH_CASE *item)
{
    int actual;
    int initial_performance = 7;
    int initial_quality = 99;

    op_sound_performance_level = initial_performance;
    op_sound_quality_level = initial_quality;
    callback_count = 0;
    expected_callback = item->callback;
    expected_quality_at_callback = item->event == 13 ? 6 : initial_quality;
    actual = op_sound_module_proc(item->event);

    check(actual == item->result, "dispatch return value");
    if (item->callback == 0) {
        check(callback_count == 0, "unknown event makes no callback");
        check(op_sound_performance_level == initial_performance &&
              op_sound_quality_level == initial_quality,
              "unknown event preserves both globals");
    } else {
        check(callback_count == 1, "handled event makes exactly one callback");
        check(op_sound_performance_level == -1000 - item->callback &&
              op_sound_quality_level == 1000 + item->callback,
              "callback global mutations are retained");
    }
}

int main(void)
{
    static const DISPATCH_CASE cases[] = {
        { 0, 0, 2 }, { 1, 1, 101 }, { 2, 2, -202 }, { 3, 0, 2 },
        { 4, 0, 2 }, { 5, 0, 2 }, { 6, 6, -606 }, { 7, 0, 2 },
        { 8, 8, 0 }, { 9, 9, 0 }, { 10, 0, 2 }, { 11, 0, 2 },
        { 12, 0, 2 }, { 13, 14, -1414 }, { 14, 14, -1414 },
        { 15, 0, 2 }, { 16, 0, 2 }, { 17, 0, 2 }, { 18, 18, 0 },
        { 19, 19, 0 }, { 20, 0, 2 }, { 21, 0, 2 }, { 22, 0, 2 },
        { 23, 0, 2 }, { 24, 0, 2 }, { 25, 25, 2501 }, { 26, 0, 2 },
        { -1, 0, 2 }, { INT_MIN, 0, 2 }, { INT_MAX, 0, 2 }
    };
    unsigned int i;

    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
        run_case(&cases[i]);
    printf("Sound module dispatcher: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
