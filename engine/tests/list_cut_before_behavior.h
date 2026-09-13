#include "../src/linked_list.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ls4_checks, ls4_failures;
static void ls4_check(int ok, int line)
{
    ++ls4_checks;
    if (!ok)
    {
        ++ls4_failures;
        printf("line %d failed\n", line);
    }
}
#define ls4_CHECK(x) ls4_check(!!(x), __LINE__)
typedef struct ls4_OWNED
{
    unsigned int before;
    OP_LIST_NODE value;
    unsigned char payload[12];
    unsigned int after;
} ls4_OWNED;
static ls4_OWNED ls4_nodes[6], ls4_expected[6];
static int ls4_rotation;
static OP_LIST_NODE *ls4_at(int index)
{
    return index < 0 ? 0 : &ls4_nodes[(index + ls4_rotation) % 6].value;
}
static void ls4_seed(int edges[6][2])
{
    int i;
    memset(ls4_nodes, 0x69, sizeof(ls4_nodes));
    for (i = 0; i < 6; ++i)
    {
        ls4_at(i)->prev = ls4_at(edges[i][0]);
        ls4_at(i)->next = ls4_at(edges[i][1]);
    }
    memcpy(ls4_expected, ls4_nodes, sizeof(ls4_nodes));
}
static void ls4_expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        ls4_expected[(i + ls4_rotation) % 6].value.prev = ls4_at(edges[i][0]);
        ls4_expected[(i + ls4_rotation) % 6].value.next = ls4_at(edges[i][1]);
    }
}
int ls4_main(void)
{
    int i;
    static int input[3][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[3][6][2] = {{{-1, -1}, {-1, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int selected[3] = {1, 0, 2};
    for (ls4_rotation = 0; ls4_rotation < 6; ++ls4_rotation)
        for (i = 0; i < 3; ++i)
        {
            ls4_seed(input[i]);
            ls4_expect_edges(output[i]);
            op_list_cut_before(ls4_at(selected[i]));
            ls4_CHECK(memcmp(ls4_nodes, ls4_expected, sizeof(ls4_nodes)) == 0);
        }
    printf("list_cut_before: %d checks, %d failures\n", ls4_checks, ls4_failures);
    return ls4_failures != 0;
}

#undef ls4_CHECK
