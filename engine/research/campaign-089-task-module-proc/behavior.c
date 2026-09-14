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
    static const int events[12] = {INT_MIN, -100, -1, 0, 1, 2, 3, 4, 5, 6, 100, INT_MAX};
    int p, e, result;
    for (p = 0; p < 16; ++p)
        for (e = 0; e < 12; ++e)
        {
            setup(p);
            if (events[e] == 1)
            {
                memset(expected, 0, sizeof(expected));
                wanted_count = 0;
                wanted_sentinel = -1;
                wanted_cursor = 0;
                wanted_current = NULL;
            }
            else if (events[e] == 5)
            {
                wanted_target = 0;
                wanted_simulation = 0;
            }
            result = op_task_module_proc(events[e]);
            CHECK(result == (events[e] == 1 ? 0 : 2));
            verify();
        }
    printf("task_module_proc: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
