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
static unsigned char pool[96], wanted_pool[96];
static void *outputs[2], *wanted_outputs[2];
static unsigned int rounded;
static int calls, retarget;
static void verify_buffers(void)
{
    verify();
    CHECK(!memcmp(pool, wanted_pool, sizeof(pool)));
    CHECK(!memcmp(outputs, wanted_outputs, sizeof(outputs)));
}
void *op_allocate(unsigned int size)
{
    CHECK(calls == 0);
    if (calls)
        exit(2);
    CHECK(size == rounded);
    if (size != rounded || size > 68)
        exit(2);
    verify_buffers();
    ++calls;
    if (retarget)
        op_task_current = wanted_current = &op_tasks[2];
    op_task_count = wanted_count = 0x77777777u;
    op_task_pending[7] = pending[7] = 0x73;
    return pool + 8;
}
int main(void)
{
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
    for (s = 0; s < 69; ++s)
        for (p = 0; p < 4; ++p)
            for (alias = 0; alias < 3; ++alias)
                for (retarget = 0; retarget < 2; ++retarget)
                {
                    setup(p);
                    op_task_current = wanted_current = &op_tasks[0];
                    rounded = rounded_sizes[s];
                    calls = 0;
                    memset(pool, 0x51 + p, sizeof(pool));
                    memcpy(wanted_pool, pool, sizeof(pool));
                    outputs[0] = wanted_outputs[0] = &tokens[0];
                    outputs[1] = wanted_outputs[1] = &tokens[1];
                    output = alias == 0 ? &outputs[0] : alias == 1 ? &op_tasks[0].data : &op_tasks[2].data;
                    op_task_alloc_state(sizes[s], output);
                    for (i = 0; i < (int)rounded; ++i)
                        wanted_pool[8 + i] = 0;
                    live = retarget ? 2 : 0;
                    expected[live].out = output;
                    expected[live].data = pool + 8;
                    if (alias == 0)
                        wanted_outputs[0] = pool + 8;
                    else
                        expected[alias == 1 ? 0 : 2].data = pool + 8;
                    CHECK(calls == 1);
                    verify_buffers();
                }
    printf("task_alloc_state: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
