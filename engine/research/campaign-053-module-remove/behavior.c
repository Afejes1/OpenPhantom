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

static int key_case, mutate, stage, selected, selected_position;
OP_MODULE *op_module_find_by_id(int id)
{
    int i;
    CHECK(stage == 0 && id == keys[key_case]);
    verify();
    selected = selected_position = -1;
    for (i = 0; i < length; ++i)
        if (expected_nodes[order[i]].value.id == id)
        {
            selected = order[i];
            selected_position = i;
            break;
        }
    ++stage;
    if (selected < 0)
        return 0;
    if (mutate)
    {
        nodes[selected].value.status.raw = expected_nodes[selected].value.status.raw = 0x81234567u;
        op_module_initialized = expected_initialized = -1;
    }
    verify();
    if (selected_position > 0)
        expected_nodes[order[selected_position - 1]].value.next =
            selected_position + 1 < length ? &nodes[order[selected_position + 1]].value : 0;
    if (selected_position + 1 < length)
        expected_nodes[order[selected_position + 1]].value.previous =
            selected_position ? &nodes[order[selected_position - 1]].value : 0;
    if (selected_position == 0)
        expected_head = length > 1 ? &nodes[order[1]].value : 0;
    if (selected_position == length - 1)
        expected_tail = length > 1 ? &nodes[order[length - 2]].value : 0;
    return &nodes[selected].value;
}
void op_release(void *pointer)
{
    CHECK(stage == 1 && selected >= 0 && selected < 5 && pointer == &nodes[selected].value);
    if (selected < 0 || selected >= 5 || pointer != &nodes[selected].value)
        return;
    verify();
    if (mutate)
    {
        nodes[selected].value.name[0] = expected_nodes[selected].value.name[0] = 'R';
        op_module_initialized = expected_initialized = INT_MIN;
        op_module_head = expected_head = &nodes[(selected + 1) % 5].value;
    }
    ++stage;
}
int main(void)
{
    int i, expected;
    for (length = 0; length < 6; ++length)
        for (rotation = 0; rotation < 5; ++rotation)
            for (duplicates = 0; duplicates < 2; ++duplicates)
                for (key_case = 0; key_case < 6; ++key_case)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        seed();
                        expected = 1;
                        for (i = 0; i < length; ++i)
                            if (expected_nodes[order[i]].value.id == keys[key_case])
                            {
                                expected = 0;
                                break;
                            }
                        stage = 0;
                        selected = -1;
                        CHECK(op_module_remove(keys[key_case]) == expected);
                        CHECK(stage == (expected ? 1 : 2));
                        verify();
                    }
    printf("module_remove: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
