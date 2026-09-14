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
typedef struct OWNED_PAYLOAD
{
    unsigned int before;
    unsigned char bytes[8];
    unsigned int after;
} OWNED_PAYLOAD;
static OWNED_NODE nodes[5], expected_nodes[5];
static OWNED_PAYLOAD payload, expected_payload;
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized;
static OP_MODULE *expected_head, *expected_tail;
static int expected_initialized;
static int kind = 1, length, rotation, duplicates, mode, result_case, key_case, payload_case;
static int order[5], plan[5], planned, calls, query;
static unsigned int event_word, payload_word;
static const int keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int results[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int words[3] = {0, 0x3f800000u, 0xffffffffu};
static void verify(void)
{
    CHECK(memcmp(nodes, expected_nodes, sizeof(nodes)) == 0);
    CHECK(memcmp(&payload, &expected_payload, sizeof(payload)) == 0);
    CHECK(op_module_head == expected_head && op_module_tail == expected_tail &&
          op_module_initialized == expected_initialized);
}
static int observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = calls, next_node;
    CHECK(calls < planned && calls < 5);
    if (calls >= planned || calls >= 5)
        exit(2);
    CHECK(node == plan[position]);
    if (node != plan[position])
        exit(2);
    verify();
    CHECK(event == (kind == 0 ? 11u : kind == 1 ? 10u : event_word));
    CHECK(argument == (kind == 0   ? payload_word
                       : kind == 1 ? (unsigned int)(query ? query : expected_nodes[node].value.id)
                                   : 0u));
    CHECK(bits == (kind == 2 ? payload_word : 0u));
    ++calls;
    if (position == 0 && mode)
    {
        op_module_head = expected_head = &nodes[(node + 2) % 5].value;
        op_module_tail = expected_tail = &nodes[(node + 3) % 5].value;
        op_module_initialized = expected_initialized = INT_MIN;
        nodes[node].value.name[0] = expected_nodes[node].value.name[0] = 'Z';
        if (kind == 0)
            payload.bytes[2] = expected_payload.bytes[2] = 0x93;
        if (query == 0 && kind != 0 && (mode == 1 || mode == 2))
        {
            next_node = planned > 1 ? plan[1] : -1;
            if (kind == 1)
                nodes[node].value.next = expected_nodes[node].value.next = next_node < 0 ? 0 : &nodes[next_node].value;
            else
                nodes[node].value.previous = expected_nodes[node].value.previous =
                    next_node < 0 ? 0 : &nodes[next_node].value;
        }
        if (mode == 3 && planned > 1)
        {
            next_node = plan[1];
            nodes[next_node].value.id = expected_nodes[next_node].value.id = INT_MIN;
        }
    }
    return results[(result_case + position) % 5];
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
    int i, n, start;
    memset(nodes, 0x57, sizeof(nodes));
    memset(&payload, 0x68, sizeof(payload));
    for (i = 0; i < 5; ++i)
    {
        nodes[i].value.next = nodes[i].value.previous = 0;
        nodes[i].value.id = keys[i];
        nodes[i].value.callback_word = (unsigned int)callbacks[i];
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
    op_module_head = expected_head = length ? &nodes[order[0]].value : 0;
    op_module_tail = expected_tail = length ? &nodes[order[length - 1]].value : 0;
    op_module_initialized = expected_initialized = 0x12345678;
    memcpy(expected_nodes, nodes, sizeof(nodes));
    memcpy(&expected_payload, &payload, sizeof(payload));
    query = keys[key_case];
    event_word = words[(payload_case + 1) % 3];
    payload_word = words[payload_case];
    planned = calls = 0;
    if (query)
    {
        for (i = 0; i < length; ++i)
            if (expected_nodes[order[i]].value.id == query)
            {
                plan[planned++] = order[i];
                break;
            }
    }
    else if (kind != 0)
    {
        for (i = 0; i < length; ++i)
        {
            start = kind == 1 ? i : length - 1 - i;
            plan[planned++] = order[start];
        }
        if (mode == 1 && planned > 0)
            planned = 1;
        if (mode == 2 && planned > 1)
        {
            for (i = 1; i + 1 < planned; ++i)
                plan[i] = plan[i + 1];
            --planned;
        }
    }
}
int main(void)
{
    int actual, expected;
    for (length = 0; length < 6; ++length)
        for (rotation = 0; rotation < 5; ++rotation)
            for (duplicates = 0; duplicates < 2; ++duplicates)
                for (key_case = 0; key_case < 6; ++key_case)
                    for (mode = 0; mode < 4; ++mode)
                        for (result_case = 0; result_case < 5; ++result_case)
                            for (payload_case = 0; payload_case < 3; ++payload_case)
                            {
                                seed();
                                expected = query ? (planned ? results[result_case] : kind == 2 ? 1 : 0) : 0;
                                actual = op_module_broadcast_save(query);
                                CHECK(actual == expected);
                                CHECK(calls == planned);
                                verify();
                            }
    printf("module_broadcast_save: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
