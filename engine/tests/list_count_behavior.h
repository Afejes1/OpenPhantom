#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls6_checks, ls6_failures;
static void ls6_check(int ok, int line)
{
    ++ls6_checks;
    if (!ok)
    {
        ++ls6_failures;
        printf("line %d failed\n", line);
    }
}
#define ls6_CHECK(x) ls6_check(!!(x), __LINE__)
typedef struct ls6_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls6_OWNED;
static ls6_OWNED ls6_nodes[6], ls6_expected[6];
static int ls6_rotation;
static OP_LIST_NODE *ls6_at(int index)
{
    return index < 0 ? 0 : &ls6_nodes[(index + ls6_rotation) % 6].value;
}
static void ls6_seed(int edges[6][2])
{
    int i;
    memset(ls6_nodes, 0x69, sizeof(ls6_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls6_at(i)->prev = ls6_at(edges[i][0]);
        ls6_at(i)->next = ls6_at(edges[i][1]);
    }
    memcpy(ls6_expected, ls6_nodes, sizeof(ls6_nodes));
}
int ls6_main(void)
{
    int edges[6][2], length, start, i, reverse, index;
    OP_LIST_NODE *input;
    for (ls6_rotation = 0; ls6_rotation < 6; ++ls6_rotation)
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
                    ls6_seed(edges);
                    input = start < length ? ls6_at(reverse ? length - 1 - start : start) : 0;
                    ls6_CHECK(op_list_count(input) == length - start);
                    ls6_CHECK(memcmp(ls6_nodes, ls6_expected, sizeof(ls6_nodes)) == 0);
                }
    printf("list_count: %d checks, %d failures\n", ls6_checks, ls6_failures);
    return ls6_failures != 0;
}

#undef ls6_CHECK
