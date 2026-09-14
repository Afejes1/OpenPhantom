#include "float_word_transport.h"
#include "../src/task_manager.h"
static int tr_update0(void){return 0;}
static int tr_update1(void){return 1;}
static int tr_update2(void){return 2;}
static int tr_update3(void){return 3;}
static OP_TASK_UPDATE tr_updates[4]={tr_update0,tr_update1,tr_update2,tr_update3};
OP_TASK_RECORD op_tasks[64];
unsigned char op_task_pending[20];
unsigned int op_task_count, op_task_cursor;
float op_task_target, op_task_simulation;
int op_task_sentinel;
OP_TASK_RECORD *op_task_current;
#include "../src/task_manager.h"
#include <stdio.h>
#include <limits.h>
static int t890_checks, t890_failures;
static void t890_check(int ok, int line)
{
    ++t890_checks;
    if (!ok)
    {
        ++t890_failures;
        printf("line%d failed\n", line);
    }
}
#define T890_CHECK(x) t890_check(!!(x), __LINE__)
static OP_TASK_RECORD t890_expected[64];
static unsigned char t890_pending[20];
static unsigned int t890_wanted_count, t890_wanted_cursor, t890_wanted_target, t890_wanted_simulation, t890_tokens[4];
static int t890_wanted_sentinel;
static OP_TASK_RECORD *t890_wanted_current;
static void t890_verify(void)
{
    T890_CHECK(!memcmp(op_tasks, t890_expected, sizeof(t890_expected)));
    T890_CHECK(!memcmp(op_task_pending, t890_pending, sizeof(t890_pending)));
    T890_CHECK(op_task_count == t890_wanted_count);
    T890_CHECK(op_task_cursor == t890_wanted_cursor);
    T890_CHECK(op_fixture_float_word(&op_task_target) == t890_wanted_target);
    T890_CHECK(op_fixture_float_word(&op_task_simulation) == t890_wanted_simulation);
    T890_CHECK(op_task_sentinel == t890_wanted_sentinel);
    T890_CHECK(op_task_current == t890_wanted_current);
}
static void t890_setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = tr_updates[i % 4];
    memcpy(t890_expected, op_tasks, sizeof(t890_expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = t890_pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = t890_wanted_count = 0xdeadbeefu;
    op_task_cursor = t890_wanted_cursor = 0xf1234567u;
    t890_wanted_target = 0x98765432u;
    op_fixture_store_float_word(&op_task_target, t890_wanted_target);
    t890_wanted_simulation = 0xc1234567u;
    op_fixture_store_float_word(&op_task_simulation, t890_wanted_simulation);
    op_task_sentinel = t890_wanted_sentinel = -719;
    op_task_current = t890_wanted_current = &op_tasks[pattern % 64];
}
static int op_test_task_module_proc(void)
{
    static const int events[12] = {INT_MIN, -100, -1, 0, 1, 2, 3, 4, 5, 6, 100, INT_MAX};
    int p, e, result;
    for (p = 0; p < 16; ++p)
        for (e = 0; e < 12; ++e)
        {
            t890_setup(p);
            if (events[e] == 1)
            {
                memset(t890_expected, 0, sizeof(t890_expected));
                t890_wanted_count = 0;
                t890_wanted_sentinel = -1;
                t890_wanted_cursor = 0;
                t890_wanted_current = NULL;
            }
            else if (events[e] == 5)
            {
                t890_wanted_target = 0;
                t890_wanted_simulation = 0;
            }
            result = op_task_module_proc(events[e]);
            T890_CHECK(result == (events[e] == 1 ? 0 : 2));
            t890_verify();
        }
    printf("task_module_proc: %d checks, %d failures\n", t890_checks, t890_failures);
    return t890_failures != 0;
}

#undef T890_CHECK

#include "../src/task_manager.h"
#include <stdio.h>
#include <limits.h>
static int t891_checks, t891_failures;
static void t891_check(int ok, int line)
{
    ++t891_checks;
    if (!ok)
    {
        ++t891_failures;
        printf("line%d failed\n", line);
    }
}
#define T891_CHECK(x) t891_check(!!(x), __LINE__)
static OP_TASK_RECORD t891_expected[64];
static unsigned char t891_pending[20];
static unsigned int t891_wanted_count, t891_wanted_cursor, t891_wanted_target, t891_wanted_simulation, t891_tokens[4];
static int t891_wanted_sentinel;
static OP_TASK_RECORD *t891_wanted_current;
static void t891_verify(void)
{
    T891_CHECK(!memcmp(op_tasks, t891_expected, sizeof(t891_expected)));
    T891_CHECK(!memcmp(op_task_pending, t891_pending, sizeof(t891_pending)));
    T891_CHECK(op_task_count == t891_wanted_count);
    T891_CHECK(op_task_cursor == t891_wanted_cursor);
    T891_CHECK(op_fixture_float_word(&op_task_target) == t891_wanted_target);
    T891_CHECK(op_fixture_float_word(&op_task_simulation) == t891_wanted_simulation);
    T891_CHECK(op_task_sentinel == t891_wanted_sentinel);
    T891_CHECK(op_task_current == t891_wanted_current);
}
static void t891_setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = tr_updates[i % 4];
    memcpy(t891_expected, op_tasks, sizeof(t891_expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = t891_pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = t891_wanted_count = 0xdeadbeefu;
    op_task_cursor = t891_wanted_cursor = 0xf1234567u;
    t891_wanted_target = 0x98765432u;
    op_fixture_store_float_word(&op_task_target, t891_wanted_target);
    t891_wanted_simulation = 0xc1234567u;
    op_fixture_store_float_word(&op_task_simulation, t891_wanted_simulation);
    op_task_sentinel = t891_wanted_sentinel = -719;
    op_task_current = t891_wanted_current = &op_tasks[pattern % 64];
}
static int op_test_task_register(void)
{
    static const unsigned int counts[6] = {0u, 1u, 63u, 64u, 0xfffffffeu, 0xffffffffu};
    int free_index, extra, p, c, k;
    OP_TASK_UPDATE callback;
    OP_TASK_RECORD *result, *wanted;
    unsigned int token_before[4];
    for (free_index = 0; free_index <= 64; ++free_index)
        for (extra = 0; extra < 2; ++extra)
            for (p = 0; p < 4; ++p)
                for (c = 0; c < 6; ++c)
                    for (k = 0; k < 3; ++k)
                    {
                        t891_setup(p);
                        memcpy(token_before, t891_tokens, sizeof(t891_tokens));
                        op_task_count = t891_wanted_count = counts[c];
                        callback = k ? tr_updates[k] : NULL;
                        if (free_index < 64)
                        {
                            op_tasks[free_index].update = NULL;
                            if (extra && free_index + 5 < 64)
                                op_tasks[free_index + 5].update = NULL;
                        }
                        memcpy(t891_expected, op_tasks, sizeof(t891_expected));
                        wanted = (OP_TASK_RECORD *)-1;
                        if (free_index < 64)
                        {
                            memset(&t891_expected[free_index], 0, sizeof(OP_TASK_RECORD));
                            memcpy(t891_expected[free_index].prefix, t891_pending, sizeof(t891_pending));
                            t891_expected[free_index].update = callback;
                            memset(t891_pending, 0, sizeof(t891_pending));
                            t891_wanted_count = counts[c] + 1u;
                            wanted = &op_tasks[free_index];
                        }
                        result = op_task_register(callback);
                        T891_CHECK(result == wanted);
                        t891_verify();
                        T891_CHECK(!memcmp(t891_tokens, token_before, sizeof(t891_tokens)));
                    }
    printf("task_register: %d checks, %d failures\n", t891_checks, t891_failures);
    return t891_failures != 0;
}

#undef T891_CHECK
