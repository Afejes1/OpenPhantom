#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls1_checks, ls1_failures;
static void ls1_check(int ok, int line)
{
    ++ls1_checks;
    if (!ok)
    {
        ++ls1_failures;
        printf("line %d failed\n", line);
    }
}
#define ls1_CHECK(x) ls1_check(!!(x), __LINE__)
typedef struct ls1_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls1_OWNED;
static ls1_OWNED ls1_nodes[6], ls1_expected[6];
static int ls1_rotation;
static OP_LIST_NODE *ls1_at(int index)
{
    return index < 0 ? 0 : &ls1_nodes[(index + ls1_rotation) % 6].value;
}
static void ls1_seed(int edges[6][2])
{
    int i;
    memset(ls1_nodes, 0x69, sizeof(ls1_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls1_at(i)->prev = ls1_at(edges[i][0]);
        ls1_at(i)->next = ls1_at(edges[i][1]);
    }
    memcpy(ls1_expected, ls1_nodes, sizeof(ls1_nodes));
}
static void ls1_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls1_expected[(i + ls1_rotation) % 6].value.prev = ls1_at(edges[i][0]);
        ls1_expected[(i + ls1_rotation) % 6].value.next = ls1_at(edges[i][1]);
    }
}
int ls1_main(void)
{
    int i;
    static int input[2][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[2][6][2] = {{{-1, 3}, {3, 2}, {1, -1}, {0, 1}, {-1, -1}, {-1, -1}},
                                  {{3, 1}, {0, 2}, {1, -1}, {-1, 0}, {-1, -1}, {-1, -1}}};
    static int selected[2] = {1, 0};
    for (ls1_rotation = 0; ls1_rotation < 6; ++ls1_rotation)
        for (i = 0; i < 2; ++i)
        {
            ls1_seed(input[i]);
            ls1_expect_edges(output[i]);
            op_list_insert_before(ls1_at(selected[i]), ls1_at(3));
            ls1_CHECK(memcmp(ls1_nodes, ls1_expected, sizeof(ls1_nodes)) == 0);
        }
    printf("list_insert_before: %d checks, %d failures\n", ls1_checks, ls1_failures);
    return ls1_failures != 0;
}

#undef ls1_CHECK
