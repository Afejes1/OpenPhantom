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
static void expect_edges(int edges[6][2])
{
    int i;
    for (i = 0; i < 6; ++i)
    {
        expected[(i + rotation) % 6].value.prev = at(edges[i][0]);
        expected[(i + rotation) % 6].value.next = at(edges[i][1]);
    }
}
int main(void)
{
    int i;
    static int input[3][6][2] = {{{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                 {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int output[3][6][2] = {{{-1, -1}, {-1, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, 2}, {1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
                                  {{-1, 1}, {0, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}}};
    static int selected[3] = {1, 0, 2};
    for (rotation = 0; rotation < 6; ++rotation)
        for (i = 0; i < 3; ++i)
        {
            seed(input[i]);
            expect_edges(output[i]);
            op_list_cut_before(at(selected[i]));
            CHECK(memcmp(nodes, expected, sizeof(nodes)) == 0);
        }
    printf("list_cut_before: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
