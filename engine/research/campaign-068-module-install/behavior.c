#include "api.h"
#include <stdio.h>
#include <stdlib.h>
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
typedef struct OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} OWNED_NODE;
static OWNED_NODE nodes[4], expected_nodes[4];
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized;
static OP_MODULE *expected_head, *expected_tail;
static int expected_initialized, expected_result;
static int fail_allocation, endpoint_case, allocator_mode, callback_mode, status_case, name_case, result_case;
static int allocations, calls, copies;
static char names[5][24] = {"", "small", "123456789012345", "1234567890123456", "12345678901234567890123"};
static char original_names[5][24];
static const int returns[6] = {0, 1, 2, -1, INT_MIN, INT_MAX};
static const unsigned int status_words[8] = {0, 1, 14, 15, 0x80000000u, 0xffffffffu, 0x12345678u, 0x76543210u};
static void verify(void)
{
    CHECK(memcmp(nodes, expected_nodes, sizeof(nodes)) == 0);
    CHECK(memcmp(names, original_names, sizeof(names)) == 0);
    CHECK(op_module_head == expected_head && op_module_tail == expected_tail &&
          op_module_initialized == expected_initialized);
}
static int callback(unsigned int, unsigned int, unsigned int);
static void expect_header(void)
{
    expected_nodes[0].value.id = (int)&nodes[0].value;
    expected_nodes[0].value.status.raw &= ~14u;
    expected_nodes[0].value.callback_word = (unsigned int)callback;
}
char *op_copy_keyframe_name(char *destination, const char *source, unsigned int count)
{
    int i, ended = 0, tail_index;
    CHECK(allocations == 1 && copies == 0 && calls == 0);
    CHECK(destination == nodes[0].value.name && source == names[name_case] && count == 16);
    if (allocations != 1 || copies || calls || destination != nodes[0].value.name || source != names[name_case] ||
        count != 16)
        exit(2);
    verify();
    ++copies;
    strncpy(destination, source, count);
    for (i = 0; i < 16; ++i)
    {
        if (!names[name_case][i])
            ended = 1;
        expected_nodes[0].value.name[i] = (char)(ended ? 0 : names[name_case][i]);
    }
    verify();
    expected_nodes[0].value.previous = expected_tail;
    expected_nodes[0].value.next = 0;
    if (expected_tail)
    {
        tail_index = expected_tail == &nodes[1].value ? 1 : expected_tail == &nodes[2].value ? 2 : 3;
        expected_nodes[tail_index].value.next = &nodes[0].value;
    }
    expected_tail = &nodes[0].value;
    if (!expected_head)
        expected_head = &nodes[0].value;
    return destination;
}
void *op_allocate(unsigned int bytes)
{
    CHECK(allocations == 0 && calls == 0 && bytes == 36);
    if (allocations || calls || bytes != 36)
        exit(2);
    verify();
    ++allocations;
    if (allocator_mode)
    {
        op_module_head = expected_head = &nodes[2].value;
        op_module_tail = expected_tail = &nodes[3].value;
        op_module_initialized = expected_initialized = INT_MIN;
    }
    if (fail_allocation)
        return 0;
    expect_header();
    return &nodes[0].value;
}
static int callback(unsigned int event, unsigned int argument, unsigned int bits)
{
    CHECK(allocations == 1 && calls == 0 && !fail_allocation);
    if (allocations != 1 || calls || fail_allocation)
        exit(2);
    verify();
    CHECK(event == 1 && argument == 0 && bits == 0);
    ++calls;
    if (callback_mode)
    {
        nodes[0].value.status.raw = expected_nodes[0].value.status.raw =
            status_words[(status_case + 3) % 8] ^ 0x00550000u;
        nodes[0].value.id = expected_nodes[0].value.id = returns[(result_case + 3) % 6];
        nodes[0].value.name[0] = expected_nodes[0].value.name[0] = 'M';
        op_module_head = expected_head = &nodes[3].value;
        op_module_tail = expected_tail = &nodes[2].value;
        op_module_initialized = expected_initialized = -1;
    }
    if (returns[result_case] == 0 || returns[result_case] == 2)
        expected_nodes[0].value.status.raw |= 1u;
    else if (returns[result_case] == 1)
        expected_nodes[0].value.status.raw &= ~1u;
    expected_result = expected_nodes[0].value.id;
    return returns[result_case];
}
static void seed(void)
{
    int i;
    memset(nodes, 0x57, sizeof(nodes));
    for (i = 0; i < 4; ++i)
    {
        nodes[i].value.next = nodes[i].value.previous = 0;
        nodes[i].value.id = returns[i];
        nodes[i].value.status.raw = status_words[(status_case + i) % 8];
        nodes[i].value.callback_word = 0x5678u;
    }
    op_module_head = expected_head = (endpoint_case & 1) ? &nodes[1].value : 0;
    op_module_tail = expected_tail = (endpoint_case & 2) ? &nodes[1].value : 0;
    op_module_initialized = expected_initialized = 0x12345678;
    memcpy(expected_nodes, nodes, sizeof(nodes));
    memcpy(original_names, names, sizeof(names));
    allocations = calls = copies = 0;
    expected_result = 0;
}
int main(void)
{
    int actual;
    for (fail_allocation = 0; fail_allocation < 2; ++fail_allocation)
        for (endpoint_case = 0; endpoint_case < 4; ++endpoint_case)
            for (allocator_mode = 0; allocator_mode < 2; ++allocator_mode)
                for (callback_mode = 0; callback_mode < 2; ++callback_mode)
                    for (status_case = 0; status_case < 8; ++status_case)
                        for (name_case = 0; name_case < 5; ++name_case)
                            for (result_case = 0; result_case < 6; ++result_case)
                            {
                                seed();
                                actual = op_module_install((unsigned int)callback, names[name_case]);
                                CHECK(actual == expected_result);
                                CHECK(allocations == 1 && calls == !fail_allocation && copies == !fail_allocation);
                                verify();
                            }
    printf("module_install: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
