static int t920_active;
#include "../src/task_manager.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
static int t920_checks, t920_failures;
static void t920_check(int ok, int line)
{
    ++t920_checks;
    if (!ok)
    {
        ++t920_failures;
        printf("line%d failed\n", line);
    }
}
#define T920_CHECK(x) t920_check(!!(x), __LINE__)
static OP_TASK_RECORD t920_expected[64];
static unsigned char t920_pending[20];
static unsigned int t920_wanted_count, t920_wanted_cursor, t920_wanted_target, t920_wanted_simulation, t920_tokens[4];
static int t920_wanted_sentinel;
static OP_TASK_RECORD *t920_wanted_current;
static void t920_verify(void)
{
    T920_CHECK(!memcmp(op_tasks, t920_expected, sizeof(t920_expected)));
    T920_CHECK(!memcmp(op_task_pending, t920_pending, sizeof(t920_pending)));
    T920_CHECK(op_task_count == t920_wanted_count);
    T920_CHECK(op_task_cursor == t920_wanted_cursor);
    T920_CHECK(op_task_target == t920_wanted_target);
    T920_CHECK(op_task_simulation == t920_wanted_simulation);
    T920_CHECK(op_task_sentinel == t920_wanted_sentinel);
    T920_CHECK(op_task_current == t920_wanted_current);
}
static void t920_setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = NULL;
    memcpy(t920_expected, op_tasks, sizeof(t920_expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = t920_pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = t920_wanted_count = 0xdeadbeefu;
    op_task_cursor = t920_wanted_cursor = 0xf1234567u;
    op_task_target = t920_wanted_target = 0x98765432u;
    op_task_simulation = t920_wanted_simulation = 0xc1234567u;
    op_task_sentinel = t920_wanted_sentinel = -719;
    op_task_current = t920_wanted_current = &op_tasks[pattern % 64];
}
static void *t920_outputs[2], *t920_wanted_outputs[2];
static int t920_slot, t920_retarget, t920_shift, t920_release_mutate, t920_count_mutate, t920_post_out,
    t920_update_calls, t920_release_calls, t920_callback_result;
static unsigned int t920_desired_cursor;
static void t920_verify_outputs(void)
{
    t920_verify();
    T920_CHECK(!memcmp(t920_outputs, t920_wanted_outputs, sizeof(t920_outputs)));
}
static int t920_update_callback(void)
{
    T920_CHECK(t920_update_calls == 0 && t920_release_calls == 0);
    if (t920_update_calls || t920_release_calls)
        exit(2);
    t920_verify_outputs();
    ++t920_update_calls;
    if (t920_retarget)
        op_task_current = t920_wanted_current = &op_tasks[(t920_slot + 1) % 64];
    if (t920_shift)
        op_task_cursor = t920_wanted_cursor = 63;
    t920_desired_cursor = t920_shift ? 63u : (unsigned int)t920_slot;
    op_task_current->out = t920_expected[t920_retarget ? (t920_slot + 1) % 64 : t920_slot].out =
        t920_post_out ? &t920_outputs[1] : NULL;
    op_task_current->data = t920_expected[t920_retarget ? (t920_slot + 1) % 64 : t920_slot].data = &t920_tokens[3];
    op_task_current->prefix[0] = t920_expected[t920_retarget ? (t920_slot + 1) % 64 : t920_slot].prefix[0] = 0x91;
    if (t920_count_mutate)
        op_task_count = t920_wanted_count = 0;
    op_task_pending[3] = t920_pending[3] = 0x77;
    return t920_callback_result;
}
static void t920_release(void *data)
{
    T920_CHECK(t920_update_calls == 1 && t920_release_calls == 0 && data == &t920_tokens[3]);
    if (t920_update_calls != 1 || t920_release_calls || data != &t920_tokens[3])
        exit(2);
    --t920_wanted_count;
    t920_verify_outputs();
    ++t920_release_calls;
    if (t920_release_mutate)
    {
        op_task_cursor = t920_wanted_cursor = 63;
        t920_desired_cursor = 63;
        op_task_current = t920_wanted_current = &op_tasks[(t920_slot + 3) % 64];
        op_task_current->data = t920_expected[(t920_slot + 3) % 64].data = &t920_tokens[0];
        op_task_pending[9] = t920_pending[9] = 0x93;
    }
}
static int op_test_task_run_all(void)
{
    int saved_route = t920_active;
    static const unsigned int counts[3] = {0u, 1u, 0xffffffffu};
    static const unsigned int countdowns[7] = {0u, 1u, 2u, 0x7fffffffu, 0x80000000u, 0x80000001u, 0xffffffffu};
    static const unsigned int decremented[7] = {0xffffffffu, 0u,          1u,         0x7ffffffeu,
                                                0x7fffffffu, 0x80000000u, 0xfffffffeu};
    static const int due[7] = {1, 1, 0, 0, 0, 1, 1};
    static const int results[6] = {INT_MIN, -1, 0, 1, 2, INT_MAX};
    static const int slots[8] = {0, 1, 7, 15, 31, 47, 62, 63};
    int a, c, d, r, pre_out, i, enabled;
    unsigned int before_tokens[4];
    t920_active = 1;
    for (a = 0; a < 8; ++a)
        for (c = 0; c < 3; ++c)
            for (d = 0; d < 7; ++d)
                for (r = 0; r < 6; ++r)
                    for (pre_out = 0; pre_out < 3; ++pre_out)
                        for (t920_retarget = 0; t920_retarget < 2; ++t920_retarget)
                            for (t920_shift = 0; t920_shift < 2; ++t920_shift)
                                for (t920_release_mutate = 0; t920_release_mutate < 2; ++t920_release_mutate)
                                    for (t920_count_mutate = 0; t920_count_mutate < 2; ++t920_count_mutate)
                                        for (t920_post_out = 0; t920_post_out < 2; ++t920_post_out)
                                        {
                                            t920_setup(a);
                                            t920_slot = slots[a];
                                            for (i = 0; i < 64; ++i)
                                                op_tasks[i].update = NULL;
                                            op_tasks[t920_slot].update = t920_update_callback;
                                            op_tasks[t920_slot].countdown = countdowns[d];
                                            op_tasks[t920_slot].out = pre_out == 0 ? NULL
                                                                      : pre_out == 1
                                                                          ? &t920_outputs[0]
                                                                          : &op_tasks[(t920_slot + 2) % 64].data;
                                            op_tasks[t920_slot].data = &t920_tokens[2];
                                            memcpy(t920_expected, op_tasks, sizeof(t920_expected));
                                            memcpy(before_tokens, t920_tokens, sizeof(t920_tokens));
                                            op_task_count = t920_wanted_count = counts[c];
                                            op_task_cursor = t920_wanted_cursor = 0x99;
                                            op_task_current = t920_wanted_current = &op_tasks[5];
                                            t920_outputs[0] = t920_wanted_outputs[0] = &t920_tokens[0];
                                            t920_outputs[1] = t920_wanted_outputs[1] = &t920_tokens[1];
                                            t920_update_calls = t920_release_calls = 0;
                                            t920_callback_result = results[r];
                                            t920_desired_cursor = (unsigned int)t920_slot;
                                            enabled = counts[c] != 0 && due[d];
                                            if (counts[c])
                                            {
                                                t920_expected[t920_slot].countdown = decremented[d];
                                                if (enabled)
                                                {
                                                    t920_wanted_cursor = (unsigned int)t920_slot;
                                                    t920_wanted_current = &op_tasks[t920_slot];
                                                    if (pre_out == 1)
                                                        t920_wanted_outputs[0] = &t920_tokens[2];
                                                    if (pre_out == 2)
                                                        t920_expected[(t920_slot + 2) % 64].data = &t920_tokens[2];
                                                }
                                            }
                                            op_task_run_all();
                                            if (enabled)
                                            {
                                                if (t920_callback_result < 0)
                                                {
                                                    if (!t920_post_out)
                                                        --t920_wanted_count;
                                                    t920_expected[t920_desired_cursor].update = NULL;
                                                }
                                                else
                                                    t920_expected[t920_desired_cursor].countdown =
                                                        (unsigned int)t920_callback_result;
                                            }
                                            if (counts[c])
                                                t920_wanted_cursor = 64;
                                            T920_CHECK(t920_update_calls == enabled);
                                            T920_CHECK(t920_release_calls ==
                                                       (enabled && t920_callback_result < 0 && t920_post_out));
                                            t920_verify_outputs();
                                            T920_CHECK(!memcmp(t920_tokens, before_tokens, sizeof(t920_tokens)));
                                        }
    /* Every possible active scan position, including null-update preservation. */
    for (t920_slot = 0; t920_slot < 64; ++t920_slot)
        for (c = 0; c < 2; ++c)
        {
            t920_setup(t920_slot);
            for (i = 0; i < 64; ++i)
                op_tasks[i].update = NULL;
            if (c)
                op_tasks[t920_slot].update = t920_update_callback;
            op_tasks[t920_slot].countdown = 2;
            memcpy(t920_expected, op_tasks, sizeof(t920_expected));
            if (c)
                t920_expected[t920_slot].countdown = 1;
            op_task_count = t920_wanted_count = 1;
            op_task_cursor = t920_wanted_cursor = 64;
            t920_update_calls = t920_release_calls = 0;
            op_task_run_all();
            T920_CHECK(t920_update_calls == 0 && t920_release_calls == 0);
            t920_verify();
        }
    t920_active = saved_route;
    printf("task_run_all: %d checks, %d failures\n", t920_checks, t920_failures);
    return t920_failures != 0;
}

#undef T920_CHECK
