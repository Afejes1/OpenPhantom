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
static OWNED_NODE nodes[5], expected_nodes[5];
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized, op_module_open;
static OP_MODULE *expected_head, *expected_tail;
static int expected_initialized, expected_open;
static int opening = 1, length, rotation, mode, result_case, status_case, init_case, open_case;
static int order[5], visits[5], plan[5], planned, calls, link_destination;
static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int status_words[8] = {0,           2,           0x80000000u, 0x80000002u,
                                             0xfffffffdu, 0xffffffffu, 0x12345670u, 0x12345672u};
static void verify(void)
{
    CHECK(memcmp(nodes, expected_nodes, sizeof(nodes)) == 0);
    CHECK(op_module_head == expected_head && op_module_tail == expected_tail);
    CHECK(op_module_initialized == expected_initialized && op_module_open == expected_open);
}
static int eligible(unsigned int word)
{
    return opening ? (word & 2) == 0 : (word & 2) != 0;
}
static int observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = calls, returned;
    CHECK(calls < planned && calls < 5);
    if (calls >= planned || calls >= 5)
        exit(2);
    CHECK(node == plan[position]);
    if (node != plan[position])
        exit(2);
    verify();
    CHECK(event == (opening ? 3u : 4u) && argument == 0 && bits == 0);
    returned = returns[(result_case + position) % 5];
    ++calls;
    if (mode)
    {
        nodes[node].value.status.raw = expected_nodes[node].value.status.raw =
            status_words[(status_case + position + 3) % 8] ^ 0x00550000u;
        nodes[node].value.name[0] = expected_nodes[node].value.name[0] = 'Q';
    }
    if (position == 0 && mode)
    {
        op_module_head = expected_head = &nodes[(node + 2) % 5].value;
        op_module_tail = expected_tail = &nodes[(node + 3) % 5].value;
        op_module_initialized = expected_initialized = INT_MIN;
        op_module_open = expected_open = returns[result_case];
        if (mode == 1 || mode == 2)
        {
            if (opening)
                nodes[node].value.previous = expected_nodes[node].value.previous =
                    link_destination < 0 ? 0 : &nodes[link_destination].value;
            else
                nodes[node].value.next = expected_nodes[node].value.next =
                    link_destination < 0 ? 0 : &nodes[link_destination].value;
        }
    }
    if (opening && returned == 0)
        expected_nodes[node].value.status.raw |= 2u;
    return returned;
}
static int callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return observe(0, a, b, c);
}
static int callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return observe(1, a, b, c);
}
static int callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return observe(2, a, b, c);
}
static int callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return observe(3, a, b, c);
}
static int callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return observe(4, a, b, c);
}
static OP_MODULE_CALLBACK callbacks[5] = {callback0, callback1, callback2, callback3, callback4};
static void seed(void)
{
    int i, n, first = -1;
    memset(nodes, 0x57, sizeof(nodes));
    for (i = 0; i < 5; ++i)
    {
        nodes[i].value.next = nodes[i].value.previous = 0;
        nodes[i].value.id = returns[i];
        nodes[i].value.callback_word = (unsigned int)callbacks[i];
        nodes[i].value.status.raw = status_words[(status_case + i) % 8];
        order[i] = (i + rotation) % 5;
    }
    for (i = 0; i < length; ++i)
    {
        n = order[i];
        nodes[n].value.previous = i ? &nodes[order[i - 1]].value : 0;
        nodes[n].value.next = i + 1 < length ? &nodes[order[i + 1]].value : 0;
    }
    op_module_head = expected_head = length ? &nodes[order[0]].value : 0;
    op_module_tail = expected_tail = length ? &nodes[order[length - 1]].value : 0;
    op_module_initialized = expected_initialized = returns[init_case];
    op_module_open = expected_open = returns[open_case];
    memcpy(expected_nodes, nodes, sizeof(nodes));
    planned = calls = 0;
    link_destination = -1;
    if (opening ? (!op_module_initialized || op_module_open) : !op_module_open)
        return;
    if (opening)
        expected_open = 1;
    for (i = 0; i < length; ++i)
        visits[i] = order[opening ? length - 1 - i : i];
    for (i = 0; i < length; ++i)
        if (eligible(expected_nodes[visits[i]].value.status.raw))
        {
            first = i;
            break;
        }
    if (first < 0)
        return;
    if (mode == 2 && first + 2 < length)
        link_destination = visits[first + 2];
    for (i = first; i < length; ++i)
    {
        if (mode == 1 && i > first)
            break;
        if (mode == 2 && i == first + 1)
            continue;
        if (eligible(expected_nodes[visits[i]].value.status.raw))
            plan[planned++] = visits[i];
    }
}
int main(void)
{
    int expected, actual;
    for (length = 0; length < 6; ++length)
        for (rotation = 0; rotation < 5; ++rotation)
            for (mode = 0; mode < 4; ++mode)
                for (result_case = 0; result_case < 5; ++result_case)
                    for (status_case = 0; status_case < 8; ++status_case)
                        for (init_case = 0; init_case < 5; ++init_case)
                            for (open_case = 0; open_case < 5; ++open_case)
                            {
                                seed();
                                expected = (returns[init_case] && !returns[open_case]) ? 0 : 1;
                                actual = op_module_open_all();
                                CHECK(actual == expected);
                                CHECK(calls == planned);
                                verify();
                            }
    printf("module_open_all: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
