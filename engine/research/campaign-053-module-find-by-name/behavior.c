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

typedef struct OWNED_NAME
{
    unsigned int before;
    char text[16];
    unsigned int after;
} OWNED_NAME;
static OWNED_NAME input, expected_input;
static int input_case, mutate, calls, next_position, expected_result, matched;
static unsigned int count;
static int index_of(OP_MODULE *p)
{
    int i;
    for (i = 0; i < 5; ++i)
        if (p == &nodes[i].value)
            return i;
    return -1;
}
int op_compare_tag(const char *left, const char *right, unsigned int n)
{
    int index, result, next;
    CHECK(!matched);
    if (matched)
        return 0;
    CHECK(next_position >= 0 && next_position < length);
    if (next_position < 0 || next_position >= length)
        return 1;
    index = order[next_position];
    CHECK(left == input.text && right == nodes[index].value.name && n == count && n <= 16);
    if (left != input.text || right != nodes[index].value.name || n > 16)
        return 1;
    verify();
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    result = strncmp(input.text, nodes[index].value.name, n);
    ++calls;
    if (mutate)
    {
        op_module_head = expected_head = 0;
        op_module_tail = expected_tail = 0;
        if (result == 0)
        {
            nodes[index].value.id = expected_nodes[index].value.id = INT_MIN + index;
        }
        else if (next_position + 2 < length)
        {
            nodes[index].value.next = expected_nodes[index].value.next = &nodes[order[next_position + 2]].value;
        }
    }
    if (result == 0)
    {
        expected_result = expected_nodes[index].value.id;
        matched = 1;
    }
    else
    {
        next = index_of(expected_nodes[index].value.next);
        if (next < 0)
            next_position = length;
        else if (next_position + 2 < length && next == order[next_position + 2])
            next_position += 2;
        else
            ++next_position;
    }
    verify();
    return result;
}
int main(void)
{
    int result;
    static const unsigned int counts[] = {0, 1, 3, 5, 15, 16};
    int count_case;
    for (length = 0; length < 6; ++length)
        for (rotation = 0; rotation < 5; ++rotation)
            for (duplicates = 0; duplicates < 2; ++duplicates)
                for (input_case = 0; input_case < 6; ++input_case)
                    for (count_case = 0; count_case < 6; ++count_case)
                        for (mutate = 0; mutate < 2; ++mutate)
                        {
                            seed();
                            memset(&input, 0x63, sizeof(input));
                            memset(input.text, 0, sizeof(input.text));
                            if (input_case < 5)
                                memcpy(input.text, names[input_case], strlen(names[input_case]) + 1);
                            else
                                memcpy(input.text, "absent", 7);
                            memcpy(&expected_input, &input, sizeof(input));
                            count = counts[count_case];
                            calls = next_position = expected_result = matched = 0;
                            result = op_module_find_by_name(input.text, count);
                            CHECK(result == expected_result);
                            CHECK(calls <= length);
                            CHECK(matched || next_position == length);
                            if (length && count == 0)
                                CHECK(calls == 1);
                            verify();
                            CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
                        }
    printf("module_find_by_name: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
