#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} OWNED;
static OWNED nodes[6], expected[6];
static int rotation;
static OP_LIST_NODE *at(int index)
{
    return index < 0 ? 0 : &nodes[(index + rotation) % 6].value;
}
static void seed(int edges[6][2])
{
    int i;
    memset(nodes, 0x69, sizeof(nodes));
    for (i = 0; i < 6; ++i)
    {
        at(i)->prev = at(edges[i][0]);
        at(i)->next = at(edges[i][1]);
    }
    memcpy(expected, nodes, sizeof(nodes));
}
int main(void)
{
    int edges[6][2], length, start, i, reverse, index;
    OP_LIST_NODE *input, *wanted;
    for (rotation = 0; rotation < 6; ++rotation)
        for (reverse = 0; reverse < 2; ++reverse)
            for (length = 0; length <= 5; ++length)
                for (start = 0; start <= length; ++start)
                {
                    for (i = 0; i < 6; ++i)
                        edges[i][0] = edges[i][1] = -1;
                    for (i = 0; i < length; ++i)
                    {
                        index = reverse ? length - 1 - i : i;
                        edges[index][0] = i ? (reverse ? length - i : i - 1) : -1;
                        edges[index][1] = i + 1 < length ? (reverse ? length - 2 - i : i + 1) : -1;
                    }
                    seed(edges);
                    input = start < length ? at(reverse ? length - 1 - start : start) : 0;
                    wanted = input ? at(reverse ? 0 : length - 1) : 0;
                    CHECK(op_list_last(input) == wanted);
                    CHECK(memcmp(nodes, expected, sizeof(nodes)) == 0);
                }
    printf("list_last: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
