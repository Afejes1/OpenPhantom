#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls7_checks, ls7_failures;
static void ls7_check(int ok, int line)
{
    ++ls7_checks;
    if (!ok)
    {
        ++ls7_failures;
        printf("line %d failed\n", line);
    }
}
#define ls7_CHECK(x) ls7_check(!!(x), __LINE__)
typedef struct ls7_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls7_OWNED;
static ls7_OWNED ls7_nodes[6], ls7_expected[6];
static int ls7_rotation;
static OP_LIST_NODE *ls7_at(int index)
{
    return index < 0 ? 0 : &ls7_nodes[(index + ls7_rotation) % 6].value;
}
static void ls7_seed(int edges[6][2])
{
    int i;
    memset(ls7_nodes, 0x69, sizeof(ls7_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls7_at(i)->prev = ls7_at(edges[i][0]);
        ls7_at(i)->next = ls7_at(edges[i][1]);
    }
    memcpy(ls7_expected, ls7_nodes, sizeof(ls7_nodes));
}
int ls7_main(void)
{
    int edges[6][2], length, start, i, reverse, index, target;
    OP_LIST_NODE *input, *wanted;
    static const int steps[7] = {INT_MIN, -1, 0, 1, 4, 5, INT_MAX};
    int step;
    for (ls7_rotation = 0; ls7_rotation < 6; ++ls7_rotation)
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
                    ls7_seed(edges);
                    input = start < length ? ls7_at(reverse ? length - 1 - start : start) : 0;
                    for (step = 0; step < 7; ++step)
                    {
                        target = steps[step] <= 0                ? start
                                 : steps[step] >= length - start ? length
                                                                 : start + steps[step];
                        wanted = target < length ? ls7_at(reverse ? length - 1 - target : target) : 0;
                        ls7_CHECK(op_list_advance(input, steps[step]) == wanted);
                        ls7_CHECK(memcmp(ls7_nodes, ls7_expected, sizeof(ls7_nodes)) == 0);
                    }
                    ls7_CHECK(memcmp(ls7_nodes, ls7_expected, sizeof(ls7_nodes)) == 0);
                }
    printf("list_advance: %d checks, %d failures\n", ls7_checks, ls7_failures);
    return ls7_failures != 0;
}

#undef ls7_CHECK
