#include "api.h"
#include <stdio.h>
#include <string.h>
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
typedef struct OWNED_MODULE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} OWNED_MODULE;
static OWNED_MODULE nodes[5], expected_nodes[5];
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized;
static OP_MODULE *expected_head, *expected_tail;
static int expected_initialized;
static const int keys[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const char *names[] = {"alpha", "beta", "gamma", "delta", "omega"};
static int order[5], length, rotation, duplicates;
static void seed(void)
{
    int i, n;
    memset(nodes, 0x57, sizeof(nodes));
    for (i = 0; i < 5; ++i)
    {
        nodes[i].value.next = nodes[i].value.previous = 0;
        nodes[i].value.id = keys[i];
        memset(nodes[i].value.name, 0, sizeof(nodes[i].value.name));
        memcpy(nodes[i].value.name, names[i], strlen(names[i]) + 1);
        order[i] = (i + rotation) % 5;
    }
    for (i = 0; i < length; ++i)
    {
        n = order[i];
        nodes[n].value.previous = i ? &nodes[order[i - 1]].value : 0;
        nodes[n].value.next = i + 1 < length ? &nodes[order[i + 1]].value : 0;
    }
    if (duplicates && length > 1)
        nodes[order[1]].value.id = nodes[order[0]].value.id;
    memcpy(expected_nodes, nodes, sizeof(nodes));
    op_module_head = expected_head = length ? &nodes[order[0]].value : 0;
    op_module_tail = expected_tail = length ? &nodes[order[length - 1]].value : 0;
    op_module_initialized = expected_initialized = 0x12345678;
}
static void verify(void)
{
    CHECK(memcmp(nodes, expected_nodes, sizeof(nodes)) == 0);
    CHECK(op_module_head == expected_head && op_module_tail == expected_tail &&
          op_module_initialized == expected_initialized);
}

static int key_case, answer, mutate, calls;
int op_module_remove(int id)
{
    CHECK(calls++ == 0 && id == keys[key_case]);
    verify();
    if (mutate)
    {
        nodes[rotation].value.callback_word = expected_nodes[rotation].value.callback_word = 0x87654321u;
        op_module_initialized = expected_initialized = 73;
    }
    return keys[answer];
}
int main(void)
{
    for (length = 0; length < 6; ++length)
        for (rotation = 0; rotation < 5; ++rotation)
            for (duplicates = 0; duplicates < 2; ++duplicates)
                for (key_case = 0; key_case < 6; ++key_case)
                    for (answer = 0; answer < 6; ++answer)
                        for (mutate = 0; mutate < 2; ++mutate)
                        {
                            seed();
                            calls = 0;
                            op_module_remove_void(keys[key_case]);
                            CHECK(calls == 1);
                            verify();
                        }
    printf("module_remove_void: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
