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

static const unsigned int words[] = {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 15, 0x80000000u, 0x80000008u, 0xffffffffu};
static const unsigned int mapped[] = {0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6, 7};
static int row, word_case, key_case, mutate, calls, request_case;
static unsigned int request;
OP_MODULE *op_module_find_by_id(int id)
{
    CHECK(calls++ == 0 && id == keys[key_case]);
    verify();
    if (row == 5)
        return 0;
    if (mutate)
    {
        nodes[row].value.status.raw = expected_nodes[row].value.status.raw = words[word_case];
        op_module_head = expected_head = &nodes[(row + 1) % 5].value;
        op_module_tail = expected_tail = &nodes[(row + 2) % 5].value;
    }
    return &nodes[row].value;
}
int main(void)
{
    unsigned int expected;
    length = 5;
    rotation = 0;
    duplicates = 0;
    for (row = 0; row < 6; ++row)
        for (word_case = 0; word_case < 15; ++word_case)
            for (key_case = 0; key_case < 6; ++key_case)
                for (request_case = 0; request_case < 34; ++request_case)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        request = request_case < 32    ? (unsigned int)request_case
                                  : request_case == 32 ? 0x80000000u
                                                       : 0xffffffffu;
                        seed();
                        if (row < 5)
                            nodes[row].value.status.raw = expected_nodes[row].value.status.raw =
                                mutate ? 0 : words[word_case];
                        calls = 0;
                        expected = row == 5 ? 0 : (mapped[words[word_case] & 15] & request & 7);
                        CHECK(op_module_get_flags(keys[key_case], request) == expected);
                        CHECK(calls == 1);
                        verify();
                    }
    printf("module_get_flags: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
