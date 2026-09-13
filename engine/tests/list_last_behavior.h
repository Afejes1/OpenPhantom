#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls8_checks, ls8_failures;
static void ls8_check(int ok, int line)
{
    ++ls8_checks;
    if (!ok)
    {
        ++ls8_failures;
        printf("line %d failed\n", line);
    }
}
#define ls8_CHECK(x) ls8_check(!!(x), __LINE__)
typedef struct ls8_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls8_OWNED;
static ls8_OWNED ls8_nodes[6], ls8_expected[6];
static int ls8_rotation;
static OP_LIST_NODE *ls8_at(int index)
{
    return index < 0 ? 0 : &ls8_nodes[(index + ls8_rotation) % 6].value;
}
static void ls8_seed(int edges[6][2])
{
    int i;
    memset(ls8_nodes, 0x69, sizeof(ls8_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls8_at(i)->prev = ls8_at(edges[i][0]);
        ls8_at(i)->next = ls8_at(edges[i][1]);
    }
    memcpy(ls8_expected, ls8_nodes, sizeof(ls8_nodes));
}
int ls8_main(void)
{
    int edges[6][2], length, start, i, reverse, index;
    OP_LIST_NODE *input, *wanted;
    for (ls8_rotation = 0; ls8_rotation < 6; ++ls8_rotation)
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
                    ls8_seed(edges);
                    input = start < length ? ls8_at(reverse ? length - 1 - start : start) : 0;
                    wanted = input ? ls8_at(reverse ? 0 : length - 1) : 0;
                    ls8_CHECK(op_list_last(input) == wanted);
                    ls8_CHECK(memcmp(ls8_nodes, ls8_expected, sizeof(ls8_nodes)) == 0);
                }
    printf("list_last: %d checks, %d failures\n", ls8_checks, ls8_failures);
    return ls8_failures != 0;
}

#undef ls8_CHECK
