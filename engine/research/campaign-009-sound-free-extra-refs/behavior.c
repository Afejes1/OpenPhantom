#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

op_sound_ref_node *op_sound_extra_head;
void *op_sound_registry;
int op_sound_ref_count;
int op_sound_extra_count;
static op_sound_ref_node nodes[3];
static char resources[3], registries[3];
static int checks, failures, event_count, events[12], current;
static int link_on_first;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) { ++failures; printf("FAIL: %s\n", description); }
}

static void event(int value)
{
    if (event_count >= 12) { check(0, "bounded event capture"); return; }
    events[event_count++] = value;
}

int op_sound_mark_resource(void *resource, int flag)
{
    event(current * 4);
    check(current >= 0 && current < 3, "mark node index bounded");
    if (current < 0 || current >= 3) return -1;
    check(resource == nodes[current].resource && flag == 1, "mark arguments");
    nodes[current].resource = &resources[(current + 1) % 3];
    return -1;
}

int op_sound_release_resource(void *resource)
{
    event(current * 4 + 1);
    check(current >= 0 && current < 3, "release node index bounded");
    if (current < 0 || current >= 3) return 0;
    check(resource == &resources[(current + 1) % 3],
          "release observes resource mutation from mark callback");
    op_sound_registry = &registries[(current + 1) % 3];
    op_sound_ref_count += 10;
    return 77;
}

int op_sound_remove_name(void *registry, char *name)
{
    event(current * 4 + 2);
    check(current >= 0 && current < 3, "remove node index bounded");
    if (current < 0 || current >= 3) return 0;
    check(registry == op_sound_registry && name == nodes[current].name,
          "remove uses current registry and node name");
    check(nodes[current].resource == 0, "resource cleared before name removal");
    if (link_on_first && current == 0)
        nodes[0].next = &nodes[1];
    return -9;
}

void op_sound_free_node(void *value)
{
    op_sound_ref_node *node;
    event(current * 4 + 3);
    check(current >= 0 && current < 3, "free node index bounded");
    if (current < 0 || current >= 3) return;
    node = (op_sound_ref_node *)value;
    if (node != &nodes[current]) {
        check(0, "free callback receives an owned node");
        return;
    }
    check(node->resource == 0, "old node cleared then freed");
    memset(node, 0xa5, sizeof *node);
    ++current;
    op_sound_extra_head = &nodes[0];
    op_sound_extra_count += 7;
}

static void run_case(int initial_count, int link_next, int expected_count)
{
    int i;
    if (initial_count < 0 || initial_count > 3 ||
        expected_count < initial_count || expected_count > 3) {
        check(0, "fixture list counts bounded");
        return;
    }
    memset(nodes, 0, sizeof nodes);
    for (i = 0; i < expected_count; ++i) {
        strcpy(nodes[i].name, "owned");
        nodes[i].resource = i == 1 ? 0 : &resources[i];
        nodes[i].next = i + 1 < initial_count ? &nodes[i + 1] : 0;
    }
    op_sound_extra_head = initial_count ? &nodes[0] : 0;
    op_sound_registry = &registries[0];
    op_sound_ref_count = 100;
    op_sound_extra_count = 50;
    event_count = 0;
    current = 0;
    link_on_first = link_next;
    op_sound_free_extra_refs();
    check(event_count == expected_count * 4, "four callbacks per reached node");
    check(current == expected_count, "exact reached-node count");
    check(op_sound_extra_head == 0, "global head cleared last");
    check(op_sound_ref_count == 100 + expected_count * 9,
          "live ref-count mutations followed by decrements");
    check(op_sound_extra_count == 50 + expected_count * 6,
          "live extra-count mutations followed by decrements");
}
int main(void)
{
    check(sizeof(op_sound_ref_node) == 64 &&
          offsetof(op_sound_ref_node, resource) == 0x38 &&
          offsetof(op_sound_ref_node, next) == 0x3c, "node ABI");
    run_case(0, 0, 0);
    run_case(1, 0, 1);
    run_case(3, 0, 3);
    run_case(1, 1, 2);
    printf("Sound free extra refs: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
