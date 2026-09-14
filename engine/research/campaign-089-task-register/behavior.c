#include "api.h"
#include <stdio.h>
#include <limits.h>
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
int main(void)
{
    static const unsigned int counts[6] = {0u, 1u, 63u, 64u, 0xfffffffeu, 0xffffffffu};
    int free_index, extra, p, c, k;
    void *callback;
    OP_TASK_RECORD *result, *wanted;
    unsigned int token_before[4];
    for (free_index = 0; free_index <= 64; ++free_index)
        for (extra = 0; extra < 2; ++extra)
            for (p = 0; p < 4; ++p)
                for (c = 0; c < 6; ++c)
                    for (k = 0; k < 3; ++k)
                    {
                        setup(p);
                        memcpy(token_before, tokens, sizeof(tokens));
                        op_task_count = wanted_count = counts[c];
                        callback = k ? &tokens[k] : NULL;
                        if (free_index < 64)
                        {
                            op_tasks[free_index].update = NULL;
                            if (extra && free_index + 5 < 64)
                                op_tasks[free_index + 5].update = NULL;
                        }
                        memcpy(expected, op_tasks, sizeof(expected));
                        wanted = (OP_TASK_RECORD *)-1;
                        if (free_index < 64)
                        {
                            memset(&expected[free_index], 0, sizeof(OP_TASK_RECORD));
                            memcpy(expected[free_index].prefix, pending, sizeof(pending));
                            expected[free_index].update = callback;
                            memset(pending, 0, sizeof(pending));
                            wanted_count = counts[c] + 1u;
                            wanted = &op_tasks[free_index];
                        }
                        result = op_task_register(callback);
                        CHECK(result == wanted);
                        verify();
                        CHECK(!memcmp(tokens, token_before, sizeof(tokens)));
                    }
    printf("task_register: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
