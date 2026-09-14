#include "api.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
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
OP_TASK_RECORD op_tasks[64];
unsigned char op_task_pending[20];
unsigned int op_task_count, op_task_cursor, op_task_target, op_task_simulation;
int op_task_sentinel;
OP_TASK_RECORD *op_task_current;
static OP_TASK_RECORD expected[64];
static unsigned char pending[20];
static unsigned int wanted_count, wanted_cursor, wanted_target, wanted_simulation, tokens[4];
static int wanted_sentinel;
static OP_TASK_RECORD *wanted_current;
static void verify(void)
{
    CHECK(!memcmp(op_tasks, expected, sizeof(expected)));
    CHECK(!memcmp(op_task_pending, pending, sizeof(pending)));
    CHECK(op_task_count == wanted_count);
    CHECK(op_task_cursor == wanted_cursor);
    CHECK(op_task_target == wanted_target);
    CHECK(op_task_simulation == wanted_simulation);
    CHECK(op_task_sentinel == wanted_sentinel);
    CHECK(op_task_current == wanted_current);
}
static void setup(int pattern)
{
    int i;
    unsigned char *b = (unsigned char *)op_tasks;
    for (i = 0; i < (int)sizeof(op_tasks); ++i)
        b[i] = (unsigned char)(i * 37 + pattern * 11 + 3);
    for (i = 0; i < 64; ++i)
        op_tasks[i].update = NULL;
    memcpy(expected, op_tasks, sizeof(expected));
    for (i = 0; i < 20; ++i)
        op_task_pending[i] = pending[i] = (unsigned char)(i * 17 + pattern * 5 + 11);
    op_task_count = wanted_count = 0xdeadbeefu;
    op_task_cursor = wanted_cursor = 0xf1234567u;
    op_task_target = wanted_target = 0x98765432u;
    op_task_simulation = wanted_simulation = 0xc1234567u;
    op_task_sentinel = wanted_sentinel = -719;
    op_task_current = wanted_current = &op_tasks[pattern % 64];
}
static void *outputs[2], *wanted_outputs[2];
static int slot, retarget, shift, release_mutate, count_mutate, post_out, update_calls, release_calls, callback_result;
static unsigned int desired_cursor;
static void verify_outputs(void)
{
    verify();
    CHECK(!memcmp(outputs, wanted_outputs, sizeof(outputs)));
}
static int update_callback(void)
{
    CHECK(update_calls == 0 && release_calls == 0);
    if (update_calls || release_calls)
        exit(2);
    verify_outputs();
    ++update_calls;
    if (retarget)
        op_task_current = wanted_current = &op_tasks[(slot + 1) % 64];
    if (shift)
        op_task_cursor = wanted_cursor = 63;
    desired_cursor = shift ? 63u : (unsigned int)slot;
    op_task_current->out = expected[retarget ? (slot + 1) % 64 : slot].out = post_out ? &outputs[1] : NULL;
    op_task_current->data = expected[retarget ? (slot + 1) % 64 : slot].data = &tokens[3];
    op_task_current->prefix[0] = expected[retarget ? (slot + 1) % 64 : slot].prefix[0] = 0x91;
    if (count_mutate)
        op_task_count = wanted_count = 0;
    op_task_pending[3] = pending[3] = 0x77;
    return callback_result;
}
void op_release(void *data)
{
    CHECK(update_calls == 1 && release_calls == 0 && data == &tokens[3]);
    if (update_calls != 1 || release_calls || data != &tokens[3])
        exit(2);
    --wanted_count;
    verify_outputs();
    ++release_calls;
    if (release_mutate)
    {
        op_task_cursor = wanted_cursor = 63;
        desired_cursor = 63;
        op_task_current = wanted_current = &op_tasks[(slot + 3) % 64];
        op_task_current->data = expected[(slot + 3) % 64].data = &tokens[0];
        op_task_pending[9] = pending[9] = 0x93;
    }
}
int main(void)
{
    static const unsigned int counts[3] = {0u, 1u, 0xffffffffu};
    static const unsigned int countdowns[7] = {0u, 1u, 2u, 0x7fffffffu, 0x80000000u, 0x80000001u, 0xffffffffu};
    static const unsigned int decremented[7] = {0xffffffffu, 0u,          1u,         0x7ffffffeu,
                                                0x7fffffffu, 0x80000000u, 0xfffffffeu};
    static const int due[7] = {1, 1, 0, 0, 0, 1, 1};
    static const int results[6] = {INT_MIN, -1, 0, 1, 2, INT_MAX};
    static const int slots[8] = {0, 1, 7, 15, 31, 47, 62, 63};
    int a, c, d, r, pre_out, i, enabled;
    unsigned int before_tokens[4];
    for (a = 0; a < 8; ++a)
        for (c = 0; c < 3; ++c)
            for (d = 0; d < 7; ++d)
                for (r = 0; r < 6; ++r)
                    for (pre_out = 0; pre_out < 3; ++pre_out)
                        for (retarget = 0; retarget < 2; ++retarget)
                            for (shift = 0; shift < 2; ++shift)
                                for (release_mutate = 0; release_mutate < 2; ++release_mutate)
                                    for (count_mutate = 0; count_mutate < 2; ++count_mutate)
                                        for (post_out = 0; post_out < 2; ++post_out)
                                        {
                                            setup(a);
                                            slot = slots[a];
                                            for (i = 0; i < 64; ++i)
                                                op_tasks[i].update = NULL;
                                            op_tasks[slot].update = update_callback;
                                            op_tasks[slot].countdown = countdowns[d];
                                            op_tasks[slot].out = pre_out == 0   ? NULL
                                                                 : pre_out == 1 ? &outputs[0]
                                                                                : &op_tasks[(slot + 2) % 64].data;
                                            op_tasks[slot].data = &tokens[2];
                                            memcpy(expected, op_tasks, sizeof(expected));
                                            memcpy(before_tokens, tokens, sizeof(tokens));
                                            op_task_count = wanted_count = counts[c];
                                            op_task_cursor = wanted_cursor = 0x99;
                                            op_task_current = wanted_current = &op_tasks[5];
                                            outputs[0] = wanted_outputs[0] = &tokens[0];
                                            outputs[1] = wanted_outputs[1] = &tokens[1];
                                            update_calls = release_calls = 0;
                                            callback_result = results[r];
                                            desired_cursor = (unsigned int)slot;
                                            enabled = counts[c] != 0 && due[d];
                                            if (counts[c])
                                            {
                                                expected[slot].countdown = decremented[d];
                                                if (enabled)
                                                {
                                                    wanted_cursor = (unsigned int)slot;
                                                    wanted_current = &op_tasks[slot];
                                                    if (pre_out == 1)
                                                        wanted_outputs[0] = &tokens[2];
                                                    if (pre_out == 2)
                                                        expected[(slot + 2) % 64].data = &tokens[2];
                                                }
                                            }
                                            op_task_run_all();
                                            if (enabled)
                                            {
                                                if (callback_result < 0)
                                                {
                                                    if (!post_out)
                                                        --wanted_count;
                                                    expected[desired_cursor].update = NULL;
                                                }
                                                else
                                                    expected[desired_cursor].countdown = (unsigned int)callback_result;
                                            }
                                            if (counts[c])
                                                wanted_cursor = 64;
                                            CHECK(update_calls == enabled);
                                            CHECK(release_calls == (enabled && callback_result < 0 && post_out));
                                            verify_outputs();
                                            CHECK(!memcmp(tokens, before_tokens, sizeof(tokens)));
                                        }
    /* Every possible active scan position, including null-update preservation. */
    for (slot = 0; slot < 64; ++slot)
        for (c = 0; c < 2; ++c)
        {
            setup(slot);
            for (i = 0; i < 64; ++i)
                op_tasks[i].update = NULL;
            if (c)
                op_tasks[slot].update = update_callback;
            op_tasks[slot].countdown = 2;
            memcpy(expected, op_tasks, sizeof(expected));
            if (c)
                expected[slot].countdown = 1;
            op_task_count = wanted_count = 1;
            op_task_cursor = wanted_cursor = 64;
            update_calls = release_calls = 0;
            op_task_run_all();
            CHECK(update_calls == 0 && release_calls == 0);
            verify();
        }
    printf("task_run_all: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
