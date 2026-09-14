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
        op_tasks[i].update = &tokens[i % 4];
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
static void *outputs[3], *wanted_outputs[3];
static int calls, retarget, post_out;
static unsigned int callback_result;
static void verify_outputs(void)
{
    verify();
    CHECK(!memcmp(outputs, wanted_outputs, sizeof(outputs)));
}
static unsigned int event_callback(void)
{
    CHECK(calls == 0);
    if (calls)
        exit(2);
    verify_outputs();
    ++calls;
    if (retarget)
        op_task_current = wanted_current = &op_tasks[2];
    op_task_current->prefix[0] = expected[retarget ? 2 : 0].prefix[0] = 0x91;
    op_task_current->data = expected[retarget ? 2 : 0].data = &tokens[3];
    op_task_current->countdown = expected[retarget ? 2 : 0].countdown = 0xf1234567u;
    op_task_current->out = expected[retarget ? 2 : 0].out = post_out == 0   ? NULL
                                                            : post_out == 1 ? &outputs[1]
                                                                            : &op_tasks[3].data;
    op_task_count = wanted_count = 0xf7777777u;
    op_task_pending[3] = pending[3] = 0x77;
    return callback_result;
}
int main(void)
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
                        for (retarget = 0; retarget < 2; ++retarget)
                            for (post_out = 0; post_out < 3; ++post_out)
                            {
                                setup(p);
                                op_task_current = wanted_current = saved = saved_kind ? &op_tasks[4] : NULL;
                                calls = 0;
                                callback_result = returns[r];
                                for (i = 0; i < 3; ++i)
                                    outputs[i] = wanted_outputs[i] = &tokens[i];
                                op_tasks[0].event = entry == 2 ? event_callback : NULL;
                                op_tasks[0].out = pre_out == 0 ? NULL : pre_out == 1 ? &outputs[0] : &op_tasks[1].data;
                                op_tasks[0].data = &tokens[2];
                                memcpy(expected, op_tasks, sizeof(expected));
                                handle = entry ? &op_tasks[0] : NULL;
                                if (entry == 2)
                                {
                                    wanted_current = &op_tasks[0];
                                    if (pre_out == 1)
                                        wanted_outputs[0] = &tokens[2];
                                    if (pre_out == 2)
                                        expected[1].data = &tokens[2];
                                }
                                result = op_task_run(handle);
                                if (entry == 2)
                                {
                                    live = retarget ? 2 : 0;
                                    if (returns[r] & 2u)
                                        expected[live].countdown = 0;
                                    if (post_out == 1)
                                        wanted_outputs[1] = NULL;
                                    if (post_out == 2)
                                        expected[3].data = NULL;
                                }
                                wanted_current = saved;
                                CHECK(result == (entry == 2 ? masked[r] : 0u));
                                CHECK(calls == (entry == 2));
                                verify_outputs();
                            }
    printf("task_run: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
