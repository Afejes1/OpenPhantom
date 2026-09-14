static int t901_active;
#include "../src/task_manager.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
static int t900_checks, t900_failures;
static void t900_check(int ok, int line)
{
    ++t900_checks;
    if (!ok)
    {
        ++t900_failures;
        printf("line%d failed\n", line);
    }
}
#define T900_CHECK(x) t900_check(!!(x), __LINE__)
static OP_TASK_RECORD t900_expected[64];
static unsigned char t900_pending[20];
static unsigned int t900_wanted_count, t900_wanted_cursor, t900_wanted_target, t900_wanted_simulation, t900_tokens[4];
static int t900_wanted_sentinel;
static OP_TASK_RECORD *t900_wanted_current;
static void t900_verify(void)
{
    T900_CHECK(!memcmp(op_tasks, t900_expected, sizeof(t900_expected)));
    T900_CHECK(!memcmp(op_task_pending, t900_pending, sizeof(t900_pending)));
    T900_CHECK(op_task_count == t900_wanted_count);
    T900_CHECK(op_task_cursor == t900_wanted_cursor);
    T900_CHECK(op_fixture_float_word(&op_task_target) == t900_wanted_target);
    T900_CHECK(op_fixture_float_word(&op_task_simulation) == t900_wanted_simulation);
    T900_CHECK(op_task_sentinel == t900_wanted_sentinel);
    T900_CHECK(op_task_current == t900_wanted_current);
}
static void t900_setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = tr_updates[i % 4];
    memcpy(t900_expected, op_tasks, sizeof(t900_expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = t900_pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = t900_wanted_count = 0xdeadbeefu;
    op_task_cursor = t900_wanted_cursor = 0xf1234567u;
    t900_wanted_target = 0x98765432u;
    op_fixture_store_float_word(&op_task_target, t900_wanted_target);
    t900_wanted_simulation = 0xc1234567u;
    op_fixture_store_float_word(&op_task_simulation, t900_wanted_simulation);
    op_task_sentinel = t900_wanted_sentinel = -719;
    op_task_current = t900_wanted_current = &op_tasks[pattern % 64];
}
static void *t900_outputs[3], *t900_wanted_outputs[3];
static int t900_calls, t900_retarget, t900_post_out;
static unsigned int t900_callback_result;
static void t900_verify_outputs(void)
{
    t900_verify();
    T900_CHECK(!memcmp(t900_outputs, t900_wanted_outputs, sizeof(t900_outputs)));
}
static unsigned int t900_event_callback(void)
{
    T900_CHECK(t900_calls == 0);
    if (t900_calls)
        exit(2);
    t900_verify_outputs();
    ++t900_calls;
    if (t900_retarget)
        op_task_current = t900_wanted_current = &op_tasks[2];
    op_task_current->prefix[0] = t900_expected[t900_retarget ? 2 : 0].prefix[0] = 0x91;
    op_task_current->data = t900_expected[t900_retarget ? 2 : 0].data = &t900_tokens[3];
    op_task_current->countdown = t900_expected[t900_retarget ? 2 : 0].countdown = 0xf1234567u;
    op_task_current->out = t900_expected[t900_retarget ? 2 : 0].out = t900_post_out == 0   ? NULL
                                                                      : t900_post_out == 1 ? &t900_outputs[1]
                                                                                           : &op_tasks[3].data;
    op_task_count = t900_wanted_count = 0xf7777777u;
    op_task_pending[3] = t900_pending[3] = 0x77;
    return t900_callback_result;
}
static int op_test_task_run(void)
{
    static const unsigned int returns[12] = {0u,          1u,          2u,          3u,     4u,    0xffffffffu,
                                             0x80000002u, 0x7fffffffu, 0xabcdef82u, 0x100u, 0xffu, 0x12345678u};
    static const unsigned int masked[12] = {0u,          1u,          0u,          1u,     4u,    0xfffffffdu,
                                            0x80000000u, 0x7ffffffdu, 0xabcdef80u, 0x100u, 0xfdu, 0x12345678u};
    int p, entry, pre_out, r, saved_kind, i, live;
    unsigned int result;
    OP_TASK_RECORD *saved, *handle;
    for (p = 0; p < 4; ++p)
        for (entry = 0; entry < 3; ++entry)
            for (pre_out = 0; pre_out < 3; ++pre_out)
                for (r = 0; r < 12; ++r)
                    for (saved_kind = 0; saved_kind < 2; ++saved_kind)
                        for (t900_retarget = 0; t900_retarget < 2; ++t900_retarget)
                            for (t900_post_out = 0; t900_post_out < 3; ++t900_post_out)
                            {
                                t900_setup(p);
                                op_task_current = t900_wanted_current = saved = saved_kind ? &op_tasks[4] : NULL;
                                t900_calls = 0;
                                t900_callback_result = returns[r];
                                for (i = 0; i < 3; ++i)
                                    t900_outputs[i] = t900_wanted_outputs[i] = &t900_tokens[i];
                                op_tasks[0].event = entry == 2 ? t900_event_callback : NULL;
                                op_tasks[0].out = pre_out == 0   ? NULL
                                                  : pre_out == 1 ? &t900_outputs[0]
                                                                 : &op_tasks[1].data;
                                op_tasks[0].data = &t900_tokens[2];
                                memcpy(t900_expected, op_tasks, sizeof(t900_expected));
                                handle = entry ? &op_tasks[0] : NULL;
                                if (entry == 2)
                                {
                                    t900_wanted_current = &op_tasks[0];
                                    if (pre_out == 1)
                                        t900_wanted_outputs[0] = &t900_tokens[2];
                                    if (pre_out == 2)
                                        t900_expected[1].data = &t900_tokens[2];
                                }
                                result = op_task_run(handle);
                                if (entry == 2)
                                {
                                    live = t900_retarget ? 2 : 0;
                                    if (returns[r] & 2u)
                                        t900_expected[live].countdown = 0;
                                    if (t900_post_out == 1)
                                        t900_wanted_outputs[1] = NULL;
                                    if (t900_post_out == 2)
                                        t900_expected[3].data = NULL;
                                }
                                t900_wanted_current = saved;
                                T900_CHECK(result == (entry == 2 ? masked[r] : 0u));
                                T900_CHECK(t900_calls == (entry == 2));
                                t900_verify_outputs();
                            }
    printf("task_run: %d checks, %d failures\n", t900_checks, t900_failures);
    return t900_failures != 0;
}

#undef T900_CHECK

#include "../src/task_manager.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
static int t901_checks, t901_failures;
static void t901_check(int ok, int line)
{
    ++t901_checks;
    if (!ok)
    {
        ++t901_failures;
        printf("line%d failed\n", line);
    }
}
#define T901_CHECK(x) t901_check(!!(x), __LINE__)
static OP_TASK_RECORD t901_expected[64];
static unsigned char t901_pending[20];
static unsigned int t901_wanted_count, t901_wanted_cursor, t901_wanted_target, t901_wanted_simulation, t901_tokens[4];
static int t901_wanted_sentinel;
static OP_TASK_RECORD *t901_wanted_current;
static void t901_verify(void)
{
    T901_CHECK(!memcmp(op_tasks, t901_expected, sizeof(t901_expected)));
    T901_CHECK(!memcmp(op_task_pending, t901_pending, sizeof(t901_pending)));
    T901_CHECK(op_task_count == t901_wanted_count);
    T901_CHECK(op_task_cursor == t901_wanted_cursor);
    T901_CHECK(op_fixture_float_word(&op_task_target) == t901_wanted_target);
    T901_CHECK(op_fixture_float_word(&op_task_simulation) == t901_wanted_simulation);
    T901_CHECK(op_task_sentinel == t901_wanted_sentinel);
    T901_CHECK(op_task_current == t901_wanted_current);
}
static void t901_setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = tr_updates[i % 4];
    memcpy(t901_expected, op_tasks, sizeof(t901_expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = t901_pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = t901_wanted_count = 0xdeadbeefu;
    op_task_cursor = t901_wanted_cursor = 0xf1234567u;
    t901_wanted_target = 0x98765432u;
    op_fixture_store_float_word(&op_task_target, t901_wanted_target);
    t901_wanted_simulation = 0xc1234567u;
    op_fixture_store_float_word(&op_task_simulation, t901_wanted_simulation);
    op_task_sentinel = t901_wanted_sentinel = -719;
    op_task_current = t901_wanted_current = &op_tasks[pattern % 64];
}
static unsigned char t901_pool[96], t901_wanted_pool[96];
static void *t901_outputs[2], *t901_wanted_outputs[2];
static unsigned int t901_rounded;
static int t901_calls, t901_retarget;
static void t901_verify_buffers(void)
{
    t901_verify();
    T901_CHECK(!memcmp(t901_pool, t901_wanted_pool, sizeof(t901_pool)));
    T901_CHECK(!memcmp(t901_outputs, t901_wanted_outputs, sizeof(t901_outputs)));
}
static void *t901_allocate(unsigned int size)
{
    T901_CHECK(t901_calls == 0);
    if (t901_calls)
        exit(2);
    T901_CHECK(size == t901_rounded);
    if (size != t901_rounded || size > 68)
        exit(2);
    t901_verify_buffers();
    ++t901_calls;
    if (t901_retarget)
        op_task_current = t901_wanted_current = &op_tasks[2];
    op_task_count = t901_wanted_count = 0x77777777u;
    op_task_pending[7] = t901_pending[7] = 0x73;
    return t901_pool + 8;
}
static int op_test_task_alloc_state(void)
{
    int saved_route = t901_active;
    static const unsigned int sizes[69] = {
        0u,  1u,  2u,  3u,  4u,  5u,  6u,  7u,  8u,  9u,  10u, 11u, 12u,         13u,         14u,        15u, 16u, 17u,
        18u, 19u, 20u, 21u, 22u, 23u, 24u, 25u, 26u, 27u, 28u, 29u, 30u,         31u,         32u,        33u, 34u, 35u,
        36u, 37u, 38u, 39u, 40u, 41u, 42u, 43u, 44u, 45u, 46u, 47u, 48u,         49u,         50u,        51u, 52u, 53u,
        54u, 55u, 56u, 57u, 58u, 59u, 60u, 61u, 62u, 63u, 64u, 65u, 4294967293u, 4294967294u, 4294967295u};
    static const unsigned int rounded_sizes[69] = {0u,  4u,  4u,  4u,  4u,  8u,  8u,  8u,  8u,  12u, 12u, 12u, 12u, 16u,
                                                   16u, 16u, 16u, 20u, 20u, 20u, 20u, 24u, 24u, 24u, 24u, 28u, 28u, 28u,
                                                   28u, 32u, 32u, 32u, 32u, 36u, 36u, 36u, 36u, 40u, 40u, 40u, 40u, 44u,
                                                   44u, 44u, 44u, 48u, 48u, 48u, 48u, 52u, 52u, 52u, 52u, 56u, 56u, 56u,
                                                   56u, 60u, 60u, 60u, 60u, 64u, 64u, 64u, 64u, 68u, 0u,  0u,  0u};
    int s, p, alias, i, live;
    void **output;
    t901_active = 1;
    for (s = 0; s < 69; ++s)
        for (p = 0; p < 4; ++p)
            for (alias = 0; alias < 3; ++alias)
                for (t901_retarget = 0; t901_retarget < 2; ++t901_retarget)
                {
                    t901_setup(p);
                    op_task_current = t901_wanted_current = &op_tasks[0];
                    t901_rounded = rounded_sizes[s];
                    t901_calls = 0;
                    memset(t901_pool, 0x51 + p, sizeof(t901_pool));
                    memcpy(t901_wanted_pool, t901_pool, sizeof(t901_pool));
                    t901_outputs[0] = t901_wanted_outputs[0] = &t901_tokens[0];
                    t901_outputs[1] = t901_wanted_outputs[1] = &t901_tokens[1];
                    output = alias == 0 ? &t901_outputs[0] : alias == 1 ? &op_tasks[0].data : &op_tasks[2].data;
                    op_task_alloc_state(sizes[s], output);
                    for (i = 0; i < (int)t901_rounded; ++i)
                        t901_wanted_pool[8 + i] = 0;
                    live = t901_retarget ? 2 : 0;
                    t901_expected[live].out = output;
                    t901_expected[live].data = t901_pool + 8;
                    if (alias == 0)
                        t901_wanted_outputs[0] = t901_pool + 8;
                    else
                        t901_expected[alias == 1 ? 0 : 2].data = t901_pool + 8;
                    T901_CHECK(t901_calls == 1);
                    t901_verify_buffers();
                }
    t901_active = saved_route;
    printf("task_alloc_state: %d checks, %d failures\n", t901_checks, t901_failures);
    return t901_failures != 0;
}

#undef T901_CHECK
